#ifndef HLK_EVENT_HOLDER_H
#define HLK_EVENT_HOLDER_H

#include "event.h"

class EventHolder {
public:
    void print();
    Hlk::Event<> onPrint;
};

#endif //HLK_EVENT_HOLDER_H