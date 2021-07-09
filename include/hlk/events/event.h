#ifndef HLK_EVENT_H
#define HLK_EVENT_H

#include "delegate.h"

#include <vector>
#include <mutex>
#include <memory>
#include <thread>

#ifdef HLK_EVENTS_DEBUG
#include <iostream>
#endif // HLK_EVENTS_DEBUG

namespace Hlk {

template <class... TParams>
class Event {
    using TDelegate = Delegate<void, TParams...>;
public:
    // Default constructor
    Event() : m_eventHandlerDeleted(false), m_destroyed(false),  m_called(false) {
        // Create mutex
        m_mutex = new std::mutex();

        // Create empty event handler collection
        m_handlers = new std::vector<TDelegate *>();
    }

    // Copy constructor
    Event(const Event &other) : m_eventHandlerDeleted(false), m_destroyed(false),
                                m_called(false) { 
        // Create mutex
        m_mutex = new std::mutex;
        
        // Copy handlers
        m_handlers = new std::vector<TDelegate *>();
        for (size_t i = 0; i < other.m_handlers->size(); ++i) {
            m_handlers->push_back((*other.m_handlers)[i]);
        }
    }

    // Move constructor
    Event(Event &&other) : m_eventHandlerDeleted(other.m_eventHandlerDeleted),
                           m_destroyed(other.m_destroyed),
                           m_called(other.m_called)  {
        // Create mutex
        m_mutex = new std::mutex();

        // Move handlers
        m_handlers = other.m_handlers;
        other.m_handlers = nullptr;
    }

    ~Event() {
        m_mutex->lock();
        /* The event is currently being processed. Some event handler caused the 
        deletion of the object containing the event */
        if (m_called) {
            m_destroyed = 1;                    
            m_mutex->unlock();
            return;
        }
        m_mutex->unlock();

        // Delete event handlers
        for (TDelegate *delegate : *m_handlers) {
            delete delegate;
        }
        delete m_handlers;
        m_handlers = nullptr;

        // Delete mutex
        delete m_mutex;
        m_mutex = nullptr;
    }

    // Add function event handler
    void addEventHandler(void (*func)(TParams...)) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle function
        auto delegate = new TDelegate();
        delegate->bind(func);

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if ((*m_handlers)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        m_handlers->push_back(delegate);
    }

    // Add method event handler
    template<class TObject>
    void addEventHandler(TObject *object, void (TObject::*method)(TParams...)) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle method
        auto delegate = new TDelegate();
        delegate->bind(object, method);

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if ((*m_handlers)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        m_handlers->push_back(delegate);
    }

    // Add lambda event handler
    template<class TLambda>
    void addEventHandler(TLambda && lambda) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle lambda
        auto delegate = new TDelegate(std::move(lambda));

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if ((*m_handlers)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        m_handlers->push_back(delegate);
    }

    // Add lambda event handler with watch
    template<class TLambda, class TClass>
    void addEventHandler(TLambda &&lambda, TClass *context) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle lambda
        auto delegate = new TDelegate(std::move(lambda), dynamic_cast<NotifierObject*>(context));
        
        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if ((*m_handlers)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        m_handlers->push_back(delegate);
    }

    // Remove function event handler
    void removeEventHandler(void (*func)(TParams...)) {
        std::unique_lock lock(*m_mutex);

        // Check event running
        if (m_called) {
            m_eventHandlerDeleted = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(func);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if ((*m_handlers)[i] == delegate) {
                delete m_handlers[i];
                m_handlers->erase(m_handlers->begin() + i);

                // Mark current to delete
                m_eventHandlerDeleted = true;

                return;
            }
        }       
    }

    // Remove method event handler
    template<class TObject>
    void removeEventHandler(TObject *object, void (TObject::*method)(TParams...)) {
        std::unique_lock lock(*m_mutex);

        // Check event running
        if (m_called) {
            m_eventHandlerDeleted = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(object, method);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if (*(*m_handlers)[i] == delegate) {
                delete (*m_handlers)[i];
                m_handlers->erase(m_handlers->begin() + i);
                return;
            }
        }   
    }

    // Remove lambda event handler
    template<class TLambda>
    void removeEventHandler(TLambda && lambda) {
        std::unique_lock lock(*m_mutex);

        // Check event running
        if (m_called) {
            m_eventHandlerDeleted = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(lambda);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < m_handlers->size(); ++i) {
            if (*(*m_handlers)[i] == delegate) {
                delete (*m_handlers)[i];
                m_handlers->erase(m_handlers->begin() + i);
                return;
            }
        }   
    }

    void operator()(TParams... params, bool async = true) {
        // Lock to avoid append or delete event handlers
        std::unique_lock lock(*m_mutex);

        // Already deleted
        if (m_destroyed) {
            return;
        }

        m_called = 1;
        m_destroyed = 0;

        /* If the handler removes the event, pointers to the allocated objects 
        will be invalid. To avoid the error of freeing non-existent resources, 
        needed to copy the pointers to the local scope of the function. */
        std::vector<TDelegate *> *handlers = m_handlers;
        std::mutex *mutex = m_mutex;

        for (unsigned int i = 0; i < handlers->size(); ++i) {
            // Check delegate valid
            if ((*(*handlers)[i]).isValid()) {                
                /* Before calling the event handler, you must unlock the mutex 
                to avoid a deadlock, since the handler can cause deletion, add 
                an event handler, or completely delete the current event */
                lock.unlock();
                if (async) {
                    std::thread(*(*handlers)[i], params...).detach();
                } else {
                    (*(*handlers)[i])(params...);
                }
                lock.lock();
            } else {
                m_eventHandlerDeleted = true;

                #ifdef HLK_EVENTS_DEBUG
                std::cout << "Dead delegate found\n";
                #endif // HLK_EVENTS_DEBUG
            }

            if (m_eventHandlerDeleted) {
                delete (*handlers)[i];
                handlers->erase(handlers->begin() + i);
                m_eventHandlerDeleted = false;
            }
        }

        // Someone trying destroyed this event during execution
        if (m_destroyed) {
            // Delete event handlers
            for (TDelegate *delegate : *handlers) {
                delete delegate;
            }
            delete handlers;

            lock.unlock();
            delete mutex;

            m_called = 0;
            return;
        }
        m_called = 0;
    }

    Event& operator=(const Event &other) {
        if (&other == this) {
            return *this;
        }

        // Delete all handlers before copying
        for (size_t i = 0; i < m_handlers->size(); ++i) {
            delete (*m_handlers)[i];
        }
        m_handlers->clear();

        // Copy handlers
        for (size_t i = 0; i < other.m_handlers->size(); ++i) {
            m_handlers->push_back( (*other.m_handlers)[i] );
        }

        return *this;
    }

protected:
    std::vector<TDelegate *> *m_handlers;
    std::mutex *m_mutex;
    bool m_eventHandlerDeleted, m_destroyed, m_called;
};

} // namespace Hlk

#endif // HLK_EVENT_H
