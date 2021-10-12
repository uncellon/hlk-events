/******************************************************************************
 * 
 * Copyright (C) 2021 Dmitry Plastinin
 * Contact: uncellon@yandex.ru, uncellon@gmail.com, uncellon@mail.ru
 * 
 * This file is part of the Hlk Events library.
 * 
 * Hlk Events is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as pubblished by the
 * Free Software Foundation, either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * Hlk Events is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more
 * details
 * 
 * You should have received a copy of the GNU Lesset General Public License
 * along with Hlk Events. If not, see <https://www.gnu.org/licenses/>.
 * 
 *****************************************************************************/

#ifndef HLK_METHOD_WRAPPER_H
#define HLK_METHOD_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TClass, class TFunction>
class MethodWrapper;

template<class TReturn, class TClass, class... TArgs>
class MethodWrapper<TClass, TReturn(TArgs...)> : public AbstractWrapper<TReturn(TArgs...)> {
    using TMWrapper = MethodWrapper<TClass, TReturn(TArgs...)>;
public:
    MethodWrapper() = default;

    MethodWrapper(const MethodWrapper &other) : m_object(other.m_object), 
                                                m_method(other.m_method) { }

    MethodWrapper(MethodWrapper && other) : m_object(other.m_object), 
                                            m_method(other.m_method) {
        other.m_object = nullptr;
        other.m_method = nullptr;
    }

    virtual ~MethodWrapper() { 
        unbind(); 
    }

    virtual TMWrapper* clone() override { 
        return new TMWrapper(*this); 
    }

    void bind(TClass *object, TReturn (TClass::*method)(TArgs...)) {
        m_object = object;
        m_method = method;
    }

    void unbind() {
        m_object = nullptr;
        m_method = nullptr;
    }

    virtual TReturn operator()(TArgs... args) override {
        return (m_object->*m_method)(args...);
    }

    MethodWrapper& operator=(const MethodWrapper &other) {
        if (this == &other) return *this;
        m_object = other.m_object;
        m_method = other.m_method;
        return *this;
    }

    MethodWrapper& operator=(MethodWrapper&& other) {
        if (this == &other) return *this;
        m_object = other.m_object;
        m_method = other.m_method;
        other.m_object = nullptr;
        other.m_method = nullptr;
        return *this;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn(TArgs...)> &other) const override {
        const TMWrapper *otherPtr = dynamic_cast<const TMWrapper *>(&other);
        return otherPtr != nullptr 
            && m_object == otherPtr->m_object 
            && m_method == otherPtr->m_method;
    }

    TClass *m_object = nullptr;
    TReturn (TClass::*m_method)(TArgs...) = nullptr;
};

} // namespace Hlk

#endif // HLK_METHOD_WRAPPER_H