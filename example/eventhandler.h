#ifndef HLK_EVENT_HANDLER
#define HLK_EVENT_HANDLER

#include <hlk/events/notifierobject.h>

class EventHandler : public Hlk::NotifierObject {
public:
    void printHandler();
};

#endif //HLK_EVENT_HANDLER