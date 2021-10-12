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

#ifndef HLK_ABSTRACT_WRAPPER_H
#define HLK_ABSTRACT_WRAPPER_H

namespace Hlk {

template<class TFunction>
class AbstractWrapper;

template<class TReturn, class... TArgs>
class AbstractWrapper<TReturn(TArgs...)> {
    using TWrapper = AbstractWrapper<TReturn(TArgs...)>;
public:
    virtual ~AbstractWrapper() { }
    virtual TWrapper *clone() = 0;    
    virtual TReturn operator()(TArgs...) = 0;

    bool operator==(const TWrapper &other) const {
        return isEquals(other);
    }

    bool operator!=(const TWrapper &other) const { 
        return !(*this == other);
    }

protected:
    virtual bool isEquals(const TWrapper &other) const = 0;
};

} // namespace Hlk

#endif // HLK_ABSTRACT_WRAPPER_H