#ifndef HLK_LAMBDA_WRAPPER_H
#define HLK_LAMBDA_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TLambda, class TReturn, class... TParams>
class LambdaWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TLWrapper = LambdaWrapper<TLambda, TReturn, TParams...>;
public:
    LambdaWrapper() 
            : lambda_(nullptr) { }

    TReturn call(TParams... params) override {
        return lambda_->operator()(params...);
    }

    void bind(TLambda && lambda) {
        lambda_ = &lambda;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
        const TLWrapper *otherPtr = dynamic_cast<const TLWrapper *>(&other);
        return otherPtr != nullptr && lambda_ == otherPtr->lambda_;
    }

    TLambda *lambda_;
};

} // namespace Hlk

#endif // HLK_LAMBDA_WRAPPER_H