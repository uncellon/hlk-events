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

#ifndef HLK_FUNCTION_WRAPPER_H
#define HLK_FUNCTION_WRAPPER_H

#include "abstractwrapper.h"

namespace Hlk {

template<class TFunction>
class FunctionWrapper;

template<class TReturn, class... TArgs>
class FunctionWrapper<TReturn(TArgs...)> : public AbstractWrapper<TReturn(TArgs...)> {
    using TFWrapper = FunctionWrapper<TReturn(TArgs...)>;
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    // Default constructor
    FunctionWrapper() = default;

    // Copy constructor
    FunctionWrapper(const FunctionWrapper &other) : m_func(other.m_func) { }

    // Move constructor
    FunctionWrapper(FunctionWrapper&& other) : m_func(other.m_func) {
        other.m_func = nullptr;
    }

    virtual ~FunctionWrapper() { unbind(); }

    /**************************************************************************
     * Public methods
     *************************************************************************/

    virtual TReturn operator()(TArgs... args) override {
        return (*m_func)(args...);
    }

    virtual TFWrapper* clone() override { return new TFWrapper(*this); }

    void bind(TReturn (*func)(TArgs...)) {
        m_func = func;
    }

    inline void unbind() {
        m_func = nullptr;
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    // Copy assignment operator
    FunctionWrapper& operator=(const FunctionWrapper &other) {
        if (this == &other) return *this;
        m_func = other.m_func;
        return *this;
    }

    // Move assignment operator
    FunctionWrapper& operator=(FunctionWrapper&& other) {
        if (this == &other) return *this;
        m_func = other.m_func;
        other.m_func = nullptr;
        return *this;
    }

protected:
    bool isEquals(const AbstractWrapper<TReturn(TArgs...)> &other) const override {
        const TFWrapper *otherPtr = dynamic_cast<const TFWrapper *>(&other);
        return otherPtr != nullptr && m_func == otherPtr->m_func;
    }

    TReturn (*m_func)(TArgs...) = nullptr;
};

} // namespace Hlk

#endif // HLK_FUNCTION_WRAPPER_H