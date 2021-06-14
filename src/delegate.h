#ifndef HLK_DELEGATE_H
#define HLK_DELEGATE_H

#include "functionwrapper.h"
#include "methodwrapper.h"
#include "lambdawrapper.h"

#include <utility>

#ifdef HLK_EVENTS_DEBUG
#include <iostream>
#endif

namespace Hlk {

template<class TReturn, class... TParams>
class Delegate {
public:
    /***************************************************************************
     * Constructors / Destructors
     **************************************************************************/

    Delegate() 
            : wrapper_(nullptr) { 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Empty delegate created (" << address << ") --" << std::endl;
        #endif
    }

    // Copy constructor
    Delegate(Delegate &) = delete;

    // Move constructor
    Delegate(Delegate && other) :
            wrapper_(other.wrapper_) {
        other.wrapper_ = nullptr;
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Delegate move constructor called (" << address << ") --" << std::endl;
        #endif
    }

    // Function constructor
    Delegate(TReturn (*func)(TParams...)) 
            : wrapper_(nullptr) { 
        bind(func); 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Function delegate created (" << address << ") --" << std::endl;
        #endif
    }

    // Method constructor
    template<class TClass>
    Delegate(TClass *object, TReturn (TClass::*method)(TParams...)) 
            : wrapper_(nullptr) { 
        bind(object, method); 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Method delegate created (" << address << ") --" << std::endl;
        #endif
    }

    // Lambda constructor
    template<class TLambda>
    Delegate(TLambda && lambda) 
            : wrapper_(nullptr) { 
        bind(std::move(lambda));
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Lambda delegate created (" << address << ") --" << std::endl;
        #endif
    }

    ~Delegate() { 
        delete(wrapper_); 
        wrapper_ = nullptr; 
        #ifdef HLK_EVENTS_DEBUG
        const void * address = static_cast<const void*>(this);
        std::cout << "-- Delegate destroyed (" << address << ") --" << std::endl;
        #endif
    }

    /***************************************************************************
     * Public methods
     **************************************************************************/

    // Bind function
    void bind(TReturn (*func)(TParams...)) {
        if (wrapper_) {
            delete wrapper_;
        }
        auto wrapper = new FunctionWrapper<TReturn, TParams...>();
        wrapper->bind(func);
        wrapper_ = wrapper;
    }

    // Bind method
    template<class TClass>
    void bind(TClass *object, TReturn (TClass::*method)(TParams...)) {
        if (wrapper_) {
            delete wrapper_;
        }
        auto wrapper = new MethodWrapper<TReturn, TClass, TParams...>();
        wrapper->bind(object, method);
        wrapper_ = wrapper;
    }

    // Bind lambda
    template<class TLambda>
    void bind(TLambda && lambda) {
        if (wrapper_) {
            delete wrapper_;
        }
        auto wrapper = new LambdaWrapper<TLambda, TReturn, TParams...>();
        wrapper->bind(std::move(lambda));
        wrapper_ = wrapper;
    }

    // Call
    TReturn operator()(TParams... params) const {
        return wrapper_->call(params...);
    }

    // Move operator
    Delegate& operator=(Delegate&& other) {
        if (wrapper_ && *this == other) {
            return *this;
        }

        delete wrapper_;
        wrapper_ = other.wrapper_;
        other.wrapper_ = nullptr;
        return *this;
    }

    AbstractWrapper<TReturn, TParams...> &wrapper() const {
        return *wrapper_;
    }

    using TDelegate = Delegate<TReturn, TParams...>;
    bool operator==(const TDelegate &other) const {
        return *wrapper_ == other.wrapper();
    }

protected:
    AbstractWrapper<TReturn, TParams...> *wrapper_;
};

} // namespace Hlk

#endif // HLK_DELEGATE_H