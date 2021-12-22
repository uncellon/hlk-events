#ifndef HLK_NOTIFIABLE_OBJECT_H
#define HLK_NOTIFIABLE_OBJECT_H

#include "eventdispatcher.h"

namespace Hlk {

class NotifiableObject {
public:
    virtual ~NotifiableObject() {
        EventDispatcher::getInstance()->notifiableDestroyed(this);
    }
};

} // namespace Hlk

#endif // HLK_NOTIFIABLE_OBJECT_H