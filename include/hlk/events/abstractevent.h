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

#ifndef HLK_ABSTRACT_EVENT_H
#define HLK_ABSTRACT_EVENT_H

#include "abstractdelegate.h"

namespace Hlk {

class AbstractEvent {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~AbstractEvent() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void removeEventHandler(AbstractDelegate *delegate) = 0;
};

} // namespace Hlk

#endif // HLK_ABSTRACT_EVENT_H