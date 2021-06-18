#ifndef HLK_METHOD_WRAPPER_H
#define HLK_METHOD_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TReturn, class TClass, class... TParams>
class MethodWrapper : public AbstractWrapper<TReturn, TParams...> {
    using TMWrapper = MethodWrapper<TReturn, TClass, TParams...>;
public:
    /***************************************************************************
     * Constructors
     **************************************************************************/

    // Default constructor
    MethodWrapper() : m_object(nullptr), m_method(nullptr) { }

    // Copy constructor
    MethodWrapper(const MethodWrapper &other) : m_object(other.m_object), 
                                                m_method(other.m_method) { }

    // Move constructor
    MethodWrapper(MethodWrapper && other) : m_object(other.m_object), 
                                            m_method(other.m_method) {
        other.m_object = nullptr;
        other.m_method = nullptr;
    }

    /***************************************************************************
     * Public methods
     **************************************************************************/

    TReturn call(TParams... params) override {
        return (m_object->*m_method)(params...);
    }

    void bind(TClass *object, TReturn (TClass::*method)(TParams...)) {
        m_object = object;
        m_method = method;
    }

    /***************************************************************************
     * Overloaded operators
     **************************************************************************/

    // Copy operator
    MethodWrapper& operator=(const MethodWrapper &other) {
        m_object = other.m_object;
        m_method = other.m_method;
    }

    // Move operator
    MethodWrapper& operator=(MethodWrapper && other) {
        m_object = other.m_object;
        m_method = other.m_method;
        other.m_object = nullptr;
        other.m_method = nullptr;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn, TParams...> &other) const override {
    const TMWrapper *otherPtr = dynamic_cast<const TMWrapper *>(&other);
    return otherPtr != nullptr && 
            m_object == otherPtr->m_object && 
            m_method == otherPtr->m_method;
    }

    TClass *m_object;
    TReturn (TClass::*m_method)(TParams...);
};

} // namespace Hlk

#endif // HLK_METHOD_WRAPPER_H