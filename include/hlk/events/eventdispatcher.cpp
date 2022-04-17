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

#include "eventdispatcher.h"
#include "object.h"
#include "abstractevent.h"

namespace Hlk {

std::mutex EventDispatcher::m_mutex;
EventDispatcher *EventDispatcher::m_instance = nullptr;

EventDispatcher *EventDispatcher::getInstance() {
    std::unique_lock lock(m_mutex);
    if (!m_instance) {
        m_instance = new EventDispatcher();
    }
    return m_instance;
}

void EventDispatcher::registerAttachment(AbstractEvent *event, Object *notifiable, AbstractDelegate *delegate) {
    std::unique_lock lock(m_vectorMutex);
    
    m_events.push_back(event);
    m_notifiables.push_back(notifiable);
    m_delegates.push_back(delegate);
}

void EventDispatcher::removeAttachment(AbstractEvent *event, AbstractDelegate *delegate) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_events.size(); ++i) {
        if (m_events[i] != event || delegate != m_delegates[i]) {
            continue;
        }
        m_events.erase(m_events.begin() + i);
        m_notifiables.erase(m_notifiables.begin() + i);
        m_delegates.erase(m_delegates.begin() + i);
        return;
    }
}

void EventDispatcher::eventDestroyed(AbstractEvent *event) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_events.size(); ++i) {
        if (m_events[i] != event) {
            continue;
        }
        m_events.erase(m_events.begin() + i);
        m_notifiables.erase(m_notifiables.begin() + i);
        m_delegates.erase(m_delegates.begin() + i);
        --i;
    }
}

void EventDispatcher::notifiableDestroyed(Object *notifiable) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_notifiables.size(); ++i) {
        if (m_notifiables[i] != notifiable) {
            continue;
        }
        m_events[i]->removeEventHandler(m_delegates[i]);
        m_events.erase(m_events.begin() + i);
        m_notifiables.erase(m_notifiables.begin() + i);
        m_delegates.erase(m_delegates.begin() + i);
        --i;
    }
}

} // namespace Hlk