/******************************************************************************
 * 
 * Copyright (C) 2021 Dmitry Plastinin
 * Contact: uncellon@yandex.ru, uncellon@gmail.com, uncellon@mail.ru
 * 
 * This file is part of the Hlk Events library.
 * 
 * Hlk Events is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as pubblished by the
 * Free Software Foundation, either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * Hlk Events is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more
 * details
 * 
 * You should have received a copy of the GNU Lesset General Public License
 * along with Hlk Events. If not, see <https://www.gnu.org/licenses/>.
 * 
 *****************************************************************************/

#ifndef HLK_EVENT_H
#define HLK_EVENT_H

#include "abstractevent.h"
#include "delegate.h"
#include "eventdispatcher.h"
#include "object.h"

#include <mutex>
#include <vector>

namespace Hlk {

template <class... TArgs>
class Event 
: public AbstractEvent,
  public UTObject {
    using TDelegate = Delegate<void(TArgs...)>;
public:
    struct EventHandlerInfo {
        TDelegate *delegate;
        EventLoop *eventLoop;
    };

    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Event() {
        // Create mutex
        m_mutex = new std::mutex();

        // Create empty event handler collection
        m_linkedHandlers = new std::vector<EventHandlerInfo>();
    }

    Event(const Event &other) { 
        // Create mutex
        m_mutex = new std::mutex();
        
        // Copy handlers
        m_linkedHandlers = new std::vector<EventHandlerInfo>();
        for (size_t i = 0; i < other.m_handlers->size(); ++i) {
            m_linkedHandlers->emplace_back((*other.m_linkedHandlers)[i]);
        }
    }

    Event(Event &&other) 
    : m_destroyed(other.m_destroyed),
      m_called(other.m_called)  {
        // Create mutex
        m_mutex = new std::mutex();

        // Move handlers
        m_linkedHandlers = other.m_linkedHandlers;
        other.m_linkedHandlers = nullptr;
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

        // Delete event handlers
        for (auto eventHandlerInfo : *m_linkedHandlers) {
            delete eventHandlerInfo.delegate;
        }
        delete m_linkedHandlers;
        m_linkedHandlers = nullptr;

        m_mutex->unlock();

        // Delete mutex
        delete m_mutex;
        m_mutex = nullptr;

        EventDispatcher::getInstance()->eventDestroyed(this);
    }

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void removeEventHandler(AbstractDelegate *delegate) override {
        removeEventHandler(dynamic_cast<TDelegate *>(delegate));
    }

    // Remove event handler, safe
    void removeEventHandler(TDelegate *delegate) {
        std::unique_lock lock(*m_mutex);
        int index = indexOfHandler(delegate);
        if (index == -1) {
            return;
        }
        delete (*m_linkedHandlers)[index].delegate;
        if (m_called) {
            (*m_linkedHandlers)[index].delegate = nullptr;
            ++m_deletedHandlersCounter;
            return;
        }
        m_linkedHandlers->erase(m_linkedHandlers->begin() + index);
    }

    /**
     * @brief Creates function delegate and attaches it to the Event
     * 
     * @param func attached function
     */
    void addEventHandler(void (*func)(TArgs...)) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle function
        auto delegate = new TDelegate(func);

        // Try to find some delegate in std::vector
        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        // Append delegate to the std::vector
        m_linkedHandlers->emplace_back( EventHandlerInfo { delegate, nullptr });
    }

    /**
     * @brief Creates method delegate and attaches it to the Event
     * 
     * The attached class must inherit from NotifiableObject. This is necessary 
     * to store all registered events (including communication with the object 
     * and delegate) in the process, which, in turn, allows you to remove 
     * handlers of destroyed objects from events.
     * 
     * @tparam TObject attached Class
     * @param object attached Object
     * @param method attached Method
     */
    template<class TObject>
    void addEventHandler(TObject *object, void (TObject::*method)(TArgs...)) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle method
        auto delegate = new TDelegate();
        delegate->bind(object, method);

        // Try to find some delegate in std::vector
        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        EventLoop *eventLoop = nullptr;

        auto o = dynamic_cast<UTObject *>(object);
        if (o) {
            eventLoop = o->eventLoop();
            EventDispatcher::getInstance()->registerAttachment(this, o, delegate);
        }

        // Append delegate to the std::vector
        m_linkedHandlers->emplace_back( EventHandlerInfo { delegate, eventLoop });
    }

    /**
     * @brief Creates lambda delegate and attaches it to the Event
     * 
     * Attaches a lambda to the Event with no context tracking. This means that 
     * calling a lambda with a lambda-capture can cause undefined behavior. For 
     * example, if instead of attaching to a method event, a lambda was attached 
     * with a captured "this", deleting the event handler object would not 
     * remove the lambda handler from the event, so the lambda could refer to an 
     * invalid "this" capture or be an invalid lambda reference.
     * 
     * @tparam TLambda lambda template
     * @param lambda attached lambda object
     */
    template<class TLambda>
    void addEventHandler(TLambda &&lambda) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle lambda
        auto delegate = new TDelegate();
        delegate->bind(std::move(lambda));

        // Try to find some delegate in std::vector
        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        // Append delegate to the std::vector
        m_linkedHandlers->emplace_back( EventHandlerInfo { delegate, nullptr } );
    }

    template<class TLambda>
    void addEventHandler(UTObject *context, TLambda &&lambda) {
        std::unique_lock lock(*m_mutex);

        // Create delegate for handle lambda
        auto delegate = new TDelegate();
        delegate->bind(std::move(lambda));

        // Try to find some delegate in std::vector
        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        EventLoop *eventLoop = nullptr;

        auto o = dynamic_cast<UTObject *>(context);
        if (o) {
            eventLoop = o->eventLoop();
            EventDispatcher::getInstance()->registerAttachment(this, o, delegate);
        }

        // Append delegate to the std::vector
        m_linkedHandlers->emplace_back( EventHandlerInfo { delegate, eventLoop } );
        // EventDispatcher::getInstance()->registerAttachment(this, context, delegate);
    }

    // Remove function event handler
    void removeEventHandler(void (*func)(TArgs...)) {
        std::unique_lock lock(*m_mutex);
        TDelegate delegate(func);
        unsafeRemoveEventHandler(&delegate);
    }

    // Remove method event handler
    template<class TObject>
    void removeEventHandler(TObject *object, void (TObject::*method)(TArgs...)) {
        std::unique_lock lock(*m_mutex);
        TDelegate delegate(object, method);
        unsafeRemoveEventHandler(&delegate);
    }

    // Remove lambda event handler
    template<class TLambda>
    void removeEventHandler(TLambda && lambda) {
        std::unique_lock lock(*m_mutex);
        TDelegate delegate(std::move(lambda));
        unsafeRemoveEventHandler(&delegate);
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    void operator()(TArgs... params, bool async = false) {
        // Lock to avoid append or delete event handlers
        std::unique_lock lock(*m_mutex);

        // Already deleted
        if (m_destroyed) {
            return;
        }

        m_called = 1;

        /* If the handler removes the event, pointers to the allocated objects 
        will be invalid. To avoid the error of freeing non-existent resources, 
        needed to copy the pointers to the local scope of the function. */
        auto *handlers = m_linkedHandlers;
        std::mutex *mutex = m_mutex;

        for (size_t i = 0; i < handlers->size(); ++i) {
            // Check that the event handler hasn't been deleted
            if ((*handlers)[i].delegate == nullptr) {
                handlers->erase(handlers->begin() + i--);
                continue;
            }

            lock.unlock();
            if ((*handlers)[i].eventLoop) {
                auto task = new Task<void(TArgs&&...)>((*handlers)[i].delegate, params...);
                task->sender = this;
                (*handlers)[i].eventLoop->pushTask(task);
            } else {
                (*handlers)[i].delegate->operator()(params...);
            }
            lock.lock();
        }

        if (m_deletedHandlersCounter) {
            for (size_t i = 0; i < handlers->size(); ++i) {
                if ((*handlers)[i].delegate == nullptr) {
                    handlers->erase(handlers->begin() + i--);
                    if (!--m_deletedHandlersCounter) {
                        break;
                    }
                    continue;
                }
            }
        }

        // Someone trying destroyed this event during execution
        if (m_destroyed) {
            // Delete event handlers
            for (auto eventHandlerInfo : *handlers) {
                delete eventHandlerInfo.delegate;
            }
            delete handlers;

            lock.unlock();
            delete mutex;

            m_called = 0;

            EventDispatcher::getInstance()->eventDestroyed(this);

            return;
        }
        m_called = 0;
    }

    // Copy assignment operator
    Event& operator=(const Event &other) {
        if (&other == this) {
            return *this;
        }

        // Delete all handlers before copying
        for (size_t i = 0; i < m_linkedHandlers->size(); ++i) {
            delete (*m_linkedHandlers)[i];
        }
        m_linkedHandlers->clear();

        // Copy handlers
        for (size_t i = 0; i < other.m_linkedHandlers->size(); ++i) {
            m_linkedHandlers->emplace_back( (*other.m_handlers)[i] );
        }

        return *this;
    }

protected:
    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    inline int indexOfHandler(TDelegate *delegate) {
        for (size_t i = 0; i < m_linkedHandlers->size(); ++i) {
            if ( *((*m_linkedHandlers)[i].delegate) != *delegate ) {
                continue;
            }
            return i;
        }
        return -1;
    }

    inline void unsafeRemoveEventHandler(TDelegate *delegate) {
        int index = indexOfHandler(delegate);
        if (index == -1) {
            return;
        }
        EventDispatcher::getInstance()->removeAttachment(this, (*m_linkedHandlers)[index].delegate);
        delete (*m_linkedHandlers)[index].delegate;
        if (m_called) {
            (*m_linkedHandlers)[index].delegate = nullptr;
            ++m_deletedHandlersCounter;
            return;
        }
        m_linkedHandlers->erase(m_linkedHandlers->begin() + index);
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    std::vector<EventHandlerInfo> *m_linkedHandlers;
    std::mutex *m_mutex = nullptr;
    unsigned int m_deletedHandlersCounter = 0;
    bool m_destroyed = false;
    bool m_called = false;
};

} // namespace Hlk

#endif // HLK_EVENT_H
