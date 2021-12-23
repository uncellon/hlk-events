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

#ifndef HLK_EVENT_DISPATCHER_H
#define HLK_EVENT_DISPATCHER_H

#include <mutex>
#include <vector>

namespace Hlk {

class AbstractEvent;
class NotifiableObject;
class AbstractDelegate;

class EventDispatcher {
public:
    /**************************************************************************
     * Methods 
     *************************************************************************/

    static EventDispatcher *getInstance();

    void registerAttachment(AbstractEvent *event, NotifiableObject *notifiable, AbstractDelegate *delegate);
    void eventDestroyed(AbstractEvent *event);
    void notifiableDestroyed(NotifiableObject *notifiable);

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    static std::mutex m_mutex;
    static EventDispatcher *m_instance;

    std::vector<AbstractEvent *> m_events;
    std::vector<NotifiableObject *> m_notifiables;
    std::vector<AbstractDelegate *> m_delegates;

private:
    /**************************************************************************
     * Constructors / Destructors (Private)
     *************************************************************************/

    EventDispatcher() = default;    
};

} // Hlk

#endif // HLK_EVENT_DISPATCHER_H