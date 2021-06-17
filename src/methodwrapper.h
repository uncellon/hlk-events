#ifndef HLK_METHOD_WRAPPER_H
#define HLK_METHOD_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TReturn, class TClass, class... TParams>
class MethodWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TMWrapper = MethodWrapper<TReturn, TClass, TParams...>;
public:
    // Default constructor
    MethodWrapper() = default;

    // Copy constructor
    MethodWrapper(const MethodWrapper &other) : object_(other.object_), 
                                                method_(other.method_) { }

    TReturn call(TParams... params) override {
        return (object_->*method_)(params...);
    }

    void bind(TClass *object, TReturn (TClass::*method)(TParams...)) {
        object_ = object;
        method_ = method;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
    const TMWrapper *otherPtr = dynamic_cast<const TMWrapper *>(&other);
    return otherPtr != nullptr && 
            object_ == otherPtr->object_ && 
            method_ == otherPtr->method_;
    }

    TClass *object_;
    TReturn (TClass::*method_)(TParams...);
};

} // namespace Hlk

#endif // HLK_METHOD_WRAPPER_H