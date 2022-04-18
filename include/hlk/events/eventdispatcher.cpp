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

std::shared_mutex EventDispatcher::m_mutex;
EventDispatcher *EventDispatcher::m_instance = nullptr;

EventDispatcher *EventDispatcher::getInstance() {
    std::unique_lock lock(m_mutex);
    if (!m_instance) {
        m_instance = new EventDispatcher();
    }
    return m_instance;
}

void EventDispatcher::registerAttachment(AbstractEvent *event, UTObject *notifiable, AbstractDelegate *delegate) {
    std::unique_lock lock(m_vectorMutex);
    
    m_attachments.emplace_back(DispatcherInfo { event, notifiable, delegate });
}

void EventDispatcher::removeAttachment(AbstractEvent *event, AbstractDelegate *delegate) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_attachments.size(); ++i) {
        if (m_attachments[i].event != event 
            || delegate != m_attachments[i].delegate) {
            continue;
        }
        m_attachments.erase(m_attachments.begin() + i);
        return;
    }
}

void EventDispatcher::eventDestroyed(AbstractEvent *event) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_attachments.size(); ++i) {
        if (m_attachments[i].event != event) {
            continue;
        }
        m_attachments.erase(m_attachments.begin() + i);
        --i;
    }
}

void EventDispatcher::clean(UTObject *notifiable) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_attachments.size(); ++i) {
        if (m_attachments[i].object != notifiable) {
            continue;
        }
        m_attachments[i].event->removeEventHandler(m_attachments[i].delegate);
        m_attachments.erase(m_attachments.begin() + i);
        --i;
    }
}

bool EventDispatcher::attachmentValid(UTObject *event, AbstractDelegate *delegate) {
    std::unique_lock lock(m_vectorMutex);

    for (size_t i = 0; i < m_attachments.size(); ++i) {
        if (m_attachments[i].event == dynamic_cast<AbstractEvent *>(event)
            && m_attachments[i].delegate == delegate) {
            return true;
        }
    }
    return false;
}

} // namespace Hlk