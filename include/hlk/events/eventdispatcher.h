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
#include <shared_mutex>
#include <vector>

namespace Hlk {

class AbstractEvent;
class UTObject;
class AbstractDelegate;

class EventDispatcher {
public:
    /**************************************************************************
     * Methods 
     *************************************************************************/

    static EventDispatcher *getInstance();

    void registerAttachment(AbstractEvent *event, UTObject *notifiable, AbstractDelegate *delegate);
    void removeAttachment(AbstractEvent *event, AbstractDelegate *delegate);

    void eventDestroyed(AbstractEvent *event);
    void clean(UTObject *notifiable);

    bool attachmentValid(UTObject *event, AbstractDelegate *delegate);

    void sharedLock() { m_mutex.lock_shared(); }
    void sharedUnlock() { m_mutex.unlock_shared(); }

protected:
    struct DispatcherInfo {
        AbstractEvent *event;
        UTObject *object;
        AbstractDelegate *delegate;
    };

    /**************************************************************************
     * Members
     *************************************************************************/

    static std::shared_mutex m_mutex;
    static EventDispatcher *m_instance;

    std::mutex m_vectorMutex;
    std::vector<DispatcherInfo> m_attachments;

private:
    /**************************************************************************
     * Constructors / Destructors (Private)
     *************************************************************************/

    EventDispatcher() = default;    
};

} // Hlk

#endif // HLK_EVENT_DISPATCHER_H