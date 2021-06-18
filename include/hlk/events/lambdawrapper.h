#ifndef HLK_LAMBDA_WRAPPER_H
#define HLK_LAMBDA_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TLambda, class TReturn, class... TParams>
class LambdaWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TLWrapper = LambdaWrapper<TLambda, TReturn, TParams...>;
public:
    /***************************************************************************
     * Constructors
     **************************************************************************/

    // Default constructor
    LambdaWrapper() : m_lambda(nullptr) { }

    // Copy constructor
    LambdaWrapper(const LambdaWrapper &other) : m_lambda(other.m_lambda) { }

    // Move constructor
    LambdaWrapper(LambdaWrapper && other) : m_lambda(other.m_lambda) {
        other.m_lambda = nullptr;
    }

    /***************************************************************************
     * Public methods
     **************************************************************************/

    TReturn call(TParams... params) override {
        return m_lambda->operator()(params...);
    }

    void bind(TLambda && lambda) {
        m_lambda = &lambda;
    }

    /***************************************************************************
     * Overloaded operators
     **************************************************************************/

    // Copy operator
    LambdaWrapper& operator=(const LambdaWrapper &other) {
        m_lambda = other.m_lambda;
    }

    // Move operator
    LambdaWrapper& operator=(LambdaWrapper && other) {
        m_lambda = other.m_lambda;
        other.m_lambda = nullptr;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
        const TLWrapper *otherPtr = dynamic_cast<const TLWrapper *>(&other);
        return otherPtr != nullptr && m_lambda == otherPtr->m_lambda;
    }

    TLambda *m_lambda;
};

} // namespace Hlk

#endif // HLK_LAMBDA_WRAPPER_H