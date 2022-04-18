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

#ifndef HLK_OBJECT_H
#define HLK_OBJECT_H

#include "eventloop.h"
#include "eventdispatcher.h"

namespace Hlk {

class UTObject {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    UTObject(EventLoop *eventLoop = nullptr);
    virtual ~UTObject();

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    EventLoop *eventLoop() const;

private:
    /**************************************************************************
     * Members
     *************************************************************************/

    EventLoop *m_eventLoop = nullptr;
};

/******************************************************************************
 * Inline definition: Accessors / Mutators
 *****************************************************************************/

inline EventLoop *UTObject::eventLoop() const { return m_eventLoop; }

} // namespace Hlk

#endif // HLK_OBJECT_H