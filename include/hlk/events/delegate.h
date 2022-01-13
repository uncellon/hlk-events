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

#ifndef HLK_DELEGATE_H
#define HLK_DELEGATE_H

#include "abstractdelegate.h"
#include "functionwrapper.h"
#include "methodwrapper.h"
#include "lambdawrapper.h"

#include <utility>

namespace Hlk {

template<class TFunction>
class Delegate;

template<class TReturn, class... TArgs>
class Delegate<TReturn(TArgs...)> : public AbstractDelegate {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Delegate() = default;

    // Auto-bind function constructor
    Delegate(TReturn (*func)(TArgs...)) { 
        bind(func);
    }

    // Auto-bind method constructor
    template<class TClass>
    Delegate(TClass *object, TReturn (TClass::*method)(TArgs...)) { 
        bind(object, method);
    }

    // Auto-bind lambda constructor
    template<class TLambda>
    Delegate(TLambda && lambda) { 
        bind(std::move(lambda)); 
    }

    // Copy constructor
    Delegate(const Delegate &other) { 
        m_wrapper = other.m_wrapper->clone(); 
    }

    // Move constructor
    Delegate(Delegate && other) {
        m_wrapper = other.m_wrapper;
        other.m_wrapper = nullptr;
    }

    ~Delegate() { 
        delete m_wrapper; 
    }

    /**************************************************************************
     * Methods
     *************************************************************************/

    // Bind function
    void bind(TReturn (*func)(TArgs...)) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        m_wrapper = new FunctionWrapper<TReturn(TArgs...)>(func);
    }

    // Bind method
    template<class TClass>
    void bind(TClass *object, TReturn (TClass::*method)(TArgs...)) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        m_wrapper = new MethodWrapper<TClass, TReturn(TArgs...)>(object, method);
    }

    // Bind lambda
    template<class TLambda>
    void bind(TLambda&& lambda) {
        if (m_wrapper) {
            delete m_wrapper;
        }
        auto wrapper = new LambdaWrapper<TLambda, TReturn(TArgs...)>();
        wrapper->bind(std::move(lambda));
        m_wrapper = wrapper;
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    TReturn operator()(TArgs... args) {
        if (!m_wrapper) {
            return TReturn();
        }
        return m_wrapper->operator()(args...);
    }

    // Copy assignment operator
    Delegate& operator=(const Delegate &other) {
        if (this == &other) {
            return *this;
        }
        m_wrapper = other.m_wrapper->clone();
        return *this;
    };

    // Move assignment operator
    Delegate& operator=(Delegate && other) {
        if (this == &other) {
            return *this;
        }
        m_wrapper = other.m_wrapper;
        other.m_wrapper = nullptr;
        return *this;
    }

    inline bool operator==(const Delegate<TReturn(TArgs...)> &other) const {
        return *m_wrapper == *(other.m_wrapper);
    }

    inline bool operator!=(const Delegate<TReturn(TArgs...)> &other) const {
        return *m_wrapper != *(other.m_wrapper);
    }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    AbstractWrapper<TReturn(TArgs...)> *m_wrapper = nullptr;
};

} // namespace Hlk

#endif // HLK_DELEGATE_H