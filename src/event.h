#ifndef HLK_EVENT_H
#define HLK_EVENT_H

#include "delegate.h"

#include <vector>
#include <mutex>

namespace Hlk {

template <class... TParams>
class Event {
    using TDelegate = Delegate<void, TParams...>;
public:

    Event() 
            : eventHandlerDeleted_(false), destroyed_(false),  called_(false) {
        handlers_ = new std::vector<TDelegate *>();
        mutex_ = new std::mutex();
    }

    ~Event() {
        mutex_->lock();
        /* The event is currently being processed. Some event handler caused the 
        deletion of the object containing the event */
        if (called_) {
            destroyed_ = 1;                    
            mutex_->unlock();
            return;
        }
        mutex_->unlock();

        // Delete event handlers
        for (TDelegate *delegate : *handlers_) {
            delete delegate;
        }
        delete handlers_;
        handlers_ = nullptr;

        // Delete mutex
        delete mutex_;
        mutex_ = nullptr;
    }


    // Add function event handler
    void addEventHandler(void (*func)(TParams...)) {
        std::unique_lock lock(*mutex_);

        // Create delegate for handle function
        auto delegate = new TDelegate();
        delegate->bind(func);

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if ((*handlers_)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        handlers_->push_back(delegate);
    }

    // Add method event handler
    template<class TObject>
    void addEventHandler(TObject *object, void (TObject::*method)(TParams...)) {
        std::unique_lock lock(*mutex_);

        // Create delegate for handle method
        auto delegate = new TDelegate();
        delegate->bind(object, method);

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if ((*handlers_)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        handlers_->push_back(delegate);
    }

    // Add lambda event handler
    template<class TLambda>
    void addEventHandler(TLambda && lambda) {
        std::unique_lock lock(*mutex_);

        // Create delegate for handle method
        auto delegate = new TDelegate(std::move(lambda));

        // Try to find some delegate in std::vector
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if ((*handlers_)[i] == delegate) {
                delete delegate;
                return;
            }
        }

        // Append delegate to the std::vector
        handlers_->push_back(delegate);
    }

    // Remove function event handler
    void removeEventHandler(void (*func)(TParams...)) {
        std::unique_lock lock(*mutex_);

        // Check event running
        if (called_) {
            eventHandlerDeleted_ = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(func);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if ((*handlers_)[i] == delegate) {
                delete handlers_[i];
                handlers_->erase(handlers_->begin() + i);

                // Mark current to delete
                eventHandlerDeleted_ = true;

                return;
            }
        }       
    }

    // Remove method event handler
    template<class TObject>
    void removeEventHandler(TObject *object, void (TObject::*method)(TParams...)) {
        std::unique_lock lock(*mutex_);

        // Check event running
        if (called_) {
            eventHandlerDeleted_ = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(object, method);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if (*(*handlers_)[i] == delegate) {
                delete (*handlers_)[i];
                handlers_->erase(handlers_->begin() + i);
                return;
            }
        }   
    }

    // Remove lambda event handler
    template<class TLambda>
    void removeEventHandler(TLambda && lambda) {
        std::unique_lock lock(*mutex_);

        // Check event running
        if (called_) {
            eventHandlerDeleted_ = 1;
            return;
        }

        // Create temp delegate
        TDelegate delegate;
        delegate.bind(lambda);

        // Try to find delegate to delete
        for (unsigned int i = 0; i < handlers_->size(); ++i) {
            if (*(*handlers_)[i] == delegate) {
                delete (*handlers_)[i];
                handlers_->erase(handlers_->begin() + i);
                return;
            }
        }   
    }

    void operator()(TParams... params) {
        // Lock to avoid append or delete event handlers
        std::unique_lock lock(*mutex_);

        // Already deleted
        if (destroyed_) {
            return;
        }

        called_ = 1;
        destroyed_ = 0;

        /* If the handler removes the event, pointers to the allocated objects 
        will be invalid. To avoid the error of freeing non-existent resources, 
        needed to copy the pointers to the local scope of the function. */
        std::vector<TDelegate *> *handlers = handlers_;
        std::mutex *mutex = mutex_;

        for (unsigned int i = 0; i < handlers->size(); ++i) {
            /* Before calling the event handler, you must unlock the mutex to 
            avoid a deadlock, since the handler can cause deletion, add an event 
            handler, or completely delete the current event */
            lock.unlock();
            (*(*handlers)[i])(params...);
            lock.lock();

            if (eventHandlerDeleted_) {
                delete (*handlers)[i];
                handlers->erase(handlers->begin() + i);
                eventHandlerDeleted_ = false;
            }
        }

        // Someone trying destroyed this event during execution
        if (destroyed_) {
            // Delete event handlers
            for (TDelegate *delegate : *handlers) {
                delete delegate;
            }
            delete handlers;

            lock.unlock();
            delete mutex;

            called_ = 0;
            return;
        }
        called_ = 0;
    }


protected:
    std::vector<TDelegate *> *handlers_;
    std::mutex *mutex_;
    bool eventHandlerDeleted_, destroyed_, called_;
};

} // namespace Hlk

#endif // HLK_EVENT_H
