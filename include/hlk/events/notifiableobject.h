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

#ifndef HLK_NOTIFIABLE_OBJECT_H
#define HLK_NOTIFIABLE_OBJECT_H

#include "eventdispatcher.h"

namespace Hlk {

class NotifiableObject {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~NotifiableObject() {
        EventDispatcher::getInstance()->notifiableDestroyed(this);
    }
};

} // namespace Hlk

#endif // HLK_NOTIFIABLE_OBJECT_H