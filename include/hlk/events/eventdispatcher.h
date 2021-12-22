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