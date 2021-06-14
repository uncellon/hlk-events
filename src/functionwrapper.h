#ifndef HLK_FUNCTION_WRAPPER_H
#define HLK_FUNCTION_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TReturn, class... TParams>
class FunctionWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TFWrapper = FunctionWrapper<TReturn, TParams...>;
public:
    TReturn call(TParams... params) override {
        return (*func_)(params...);
    }

    void bind(TReturn (*func)(TParams...)) {
        func_ = func;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
        const TFWrapper *otherPtr = dynamic_cast<const TFWrapper *>(&other);
        return otherPtr != nullptr && func_ == otherPtr->func_;
    }

    TReturn (*func_)(TParams...);
};

} // namespace Hlk

#endif // HLK_FUNCTION_WRAPPER_H