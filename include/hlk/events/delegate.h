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

    Delegate() { 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Empty delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Copy constructor
    Delegate(const Delegate &other) { 
        m_wrapper = other.m_wrapper;
        m_context = other.m_context;
        m_withContext = other.m_withContext;
    }

    // Move constructor
    Delegate(Delegate && other) : m_wrapper(other.m_wrapper), m_context(other.m_context) {
        other.m_wrapper = nullptr;
        other.m_context.reset();

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Delegate move constructor called (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Function constructor
    Delegate(TReturn (*func)(TParams...)) {
        bind(func);

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Function delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Method constructor
    template<class TClass>
    Delegate(TClass *object, TReturn (TClass::*method)(TParams...)) { 
        bind(object, method); 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Method delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Lambda constructor
    template<class TLambda>
    explicit Delegate(TLambda && lambda) { 
        bind(std::move(lambda));

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Lambda delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    // Lambda constructor with watched object
    template<class TLambda>
    Delegate(TLambda && lambda, NotifierObject *object) { 
        bind(std::move(lambda), object);

        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Lambda delegate created (" << address << ") --" << std::endl;
        #endif // HLK_EVENTS_DEBUG
    }

    ~Delegate() { 
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
        using FW = FunctionWrapper<TReturn, TParams...>;
        auto wrapper = std::make_shared<FW>(FW());
        wrapper->bind(func);
        m_wrapper = wrapper;
    }

    // Bind method
    template<class TClass>
    void bind(TClass *object, TReturn (TClass::*method)(TParams...)) {
        using MW = MethodWrapper<TReturn, TClass, TParams...>;
        auto wrapper = std::make_shared<MW>(MW());
        wrapper->bind(object, method);
        m_wrapper = wrapper;

        // Add watch to the object
        setContext(dynamic_cast<NotifierObject*>(object));
    }

    // Bind lambda
    template<class TLambda>
    void bind(TLambda && lambda) {
        using LW = LambdaWrapper<TLambda, TReturn, TParams...>;
        auto wrapper = std::make_shared<LW>(LW());
        wrapper->bind(std::move(lambda));
        m_wrapper = wrapper;
    }

    // Bind lambda with watching object
    template<class TLambda>
    void bind(TLambda && lambda, NotifierObject *context) {
        using LW = LambdaWrapper<TLambda, TReturn, TParams...>;
        auto wrapper = std::make_shared<LW>(LW());
        wrapper->bind(std::move(lambda));
        m_wrapper = wrapper;
        setContext(context);
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
    void setContext(NotifierObject *object) {
        m_context = object->m_selfSharedPointer;
        m_withContext = true;
    }

    // Call
    TReturn operator()(TParams... params) const {
        if (m_wrapper) {
            return m_wrapper->call(params...);
        }
    }

    // Move assigment operator
    Delegate& operator=(Delegate&& other) {
        if (&other == this) {
            return *this;
        }

        m_wrapper = other.m_wrapper;
        m_context = other.m_context;
        other.m_wrapper.reset();
        other.m_context.reset();

        return *this;
    }

    // Copy assigment operator
    Delegate& operator=(const Delegate &other) {
        if (&other == this) {
            return *this;
        }

        m_wrapper = other.m_wrapper;
        m_context = other.m_context;

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
        if (m_withContext && m_context.expired()) {
            return false;
        }
        return true;
    }

protected:
    std::shared_ptr<AbstractWrapper<TReturn, TParams...>> m_wrapper;
    std::weak_ptr<NotifierObject> m_context;
    bool m_withContext = false;
};

} // namespace Hlk

#endif // HLK_DELEGATE_H