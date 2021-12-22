#include "eventdispatcher.h"
#include "notifiableobject.h"
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

void EventDispatcher::registerAttachment(AbstractEvent *event, NotifiableObject *notifiable, AbstractDelegate *delegate) {
    m_events.push_back(event);
    m_notifiables.push_back(notifiable);
    m_delegates.push_back(delegate);
}

void EventDispatcher::eventDestroyed(AbstractEvent *event) {
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

void EventDispatcher::notifiableDestroyed(NotifiableObject *notifiable) {
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