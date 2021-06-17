#ifndef HLK_DELEGATE_H
#define HLK_DELEGATE_H

#include "functionwrapper.h"
#include "methodwrapper.h"
#include "lambdawrapper.h"
#include "notifierobject.h"

#include <utility>

#ifdef HLK_EVENTS_DEBUG
#include <iostream>
#endif // HLK_EVENTS_DEBUG

namespace Hlk {

template<class TReturn, class... TParams>
class Delegate {
public:
    /***************************************************************************
     * Constructors / Destructors
     **************************************************************************/

    Delegate() : m_wrapper(nullptr) { 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Empty delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Copy constructor
    Delegate(const Delegate &other) : m_wrapper(other.m_wrapper) { }

    // Move constructor
    Delegate(Delegate && other) : m_wrapper(other.m_wrapper) {
        other.m_wrapper = nullptr;
        other.m_weakPointer = nullptr;

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Delegate move constructor called (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Function constructor
    Delegate(TReturn (*func)(TParams...)) : m_wrapper(nullptr) {
        bind(func);

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Function delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Method constructor
    template<class TClass>
    Delegate(TClass *object, TReturn (TClass::*method)(TParams...)) : m_wrapper(nullptr) { 
        bind(object, method); 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Method delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Lambda constructor
    template<class TLambda>
    Delegate(TLambda && lambda) : m_wrapper(nullptr) { 
        bind(std::move(lambda));

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Lambda delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Lambda constructor with watched object
    template<class TLambda>
    Delegate(TLambda && lambda, NotifierObject *object) : m_wrapper(nullptr) { 
        bind(std::move(lambda), object);

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Lambda delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    ~Delegate() { 
        delete(m_wrapper); 
        m_wrapper = nullptr;

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Delegate destroyed (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    /***************************************************************************
     * Public methods
     **************************************************************************/

    // Bind function
    void bind(TReturn (*func)(TParams...)) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        auto wrapper = new FunctionWrapper<TReturn, TParams...>();
        wrapper->bind(func);
        m_wrapper = wrapper;
    }

    // Bind method
    template<class TClass>
    void bind(TClass *object, TReturn (TClass::*method)(TParams...)) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        auto wrapper = new MethodWrapper<TReturn, TClass, TParams...>();
        wrapper->bind(object, method);
        m_wrapper = wrapper;

        // Add watch to the object
        watchObject(dynamic_cast<NotifierObject*>(object));
    }

    // Bind lambda
    template<class TLambda>
    void bind(TLambda && lambda) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        auto wrapper = new LambdaWrapper<TLambda, TReturn, TParams...>();
        wrapper->bind(std::move(lambda));
        m_wrapper = wrapper;
    }

    // Bind lambda with watching object
    template<class TLambda>
    void bind(TLambda && lambda, NotifierObject *watchedObject) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        auto wrapper = new LambdaWrapper<TLambda, TReturn, TParams...>();
        wrapper->bind(std::move(lambda));
        m_wrapper = wrapper;

        watchObject(watchedObject);
    }

    /**
     * @brief add binding to object lifecycle
     * 
     * @param object the object to which the delegate is bound
     * 
     * Problem: if the delegate refers to a method on an object that has already 
     * been destroyed, then this can cause unexpected behavior. To avoid this, 
     * add a weak pointer to the tracked object. Then it becomes possible to 
     * check whether it is valid by isValid(...) method before executing the 
     * delegate.
     */
    void watchObject(NotifierObject *object) {
        m_weakPointer = object->m_selfSharedPointer;
    }

    // Call
    TReturn operator()(TParams... params) const {
        return m_wrapper->call(params...);
    }

    // Move operator
    Delegate& operator=(Delegate&& other) {
        if (&other == this) {
            return *this;
        }

        delete m_wrapper;
        m_wrapper = other.m_wrapper;
        other.m_wrapper = nullptr;

        return *this;
    }

    AbstractWrapper<TReturn, TParams...> &wrapper() const {
        return *m_wrapper;
    }

    using TDelegate = Delegate<TReturn, TParams...>;
    bool operator==(const TDelegate &other) const {
        return *m_wrapper == other.wrapper();
    }

    bool isValid() const {
        if (m_weakPointer.use_count() == 1) {
            return false;
        }
        return true;
    }

protected:
    AbstractWrapper<TReturn, TParams...> *m_wrapper;
    std::shared_ptr<NotifierObject> m_weakPointer;
};

} // namespace Hlk

#endif // HLK_DELEGATE_H