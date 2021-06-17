#ifndef HLK_FUNCTION_WRAPPER_H
#define HLK_FUNCTION_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TReturn, class... TParams>
class FunctionWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TFWrapper = FunctionWrapper<TReturn, TParams...>;
public:
    /***************************************************************************
     * Constructors
     **************************************************************************/

    // Default constructor
    FunctionWrapper() : m_func(nullptr) { }

    // Copy constructor
    FunctionWrapper(const FunctionWrapper &other) : m_func(other.m_func) { }

    // Move constructor
    FunctionWrapper(FunctionWrapper && other) : m_func(other.m_func) {
        other.m_func = nullptr;
    }

    /***************************************************************************
     * Public methods
     **************************************************************************/

    TReturn call(TParams... params) override {
        return (*m_func)(params...);
    }

    void bind(TReturn (*func)(TParams...)) {
        m_func = func;
    }

    /***************************************************************************
     * Overloaded operators
     **************************************************************************/

    // Copy operator
    FunctionWrapper& operator=(const FunctionWrapper &other) {
        m_func = other.m_func;
    }

    // Move operator
    FunctionWrapper& operator=(FunctionWrapper && other) {
        m_func = other.m_func;
        other.m_func = nullptr;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
        const TFWrapper *otherPtr = dynamic_cast<const TFWrapper *>(&other);
        return otherPtr != nullptr && m_func == otherPtr->m_func;
    }

    TReturn (*m_func)(TParams...);
};

} // namespace Hlk

#endif // HLK_FUNCTION_WRAPPER_H