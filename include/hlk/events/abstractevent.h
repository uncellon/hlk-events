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