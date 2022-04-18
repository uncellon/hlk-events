#include <hlk/events/event.h>
#include <iostream>

using namespace Hlk;
using namespace std;

unsigned int counter = 0;

class A {
public:
    void fireEvent() { onTriggered(); }
    
    Event<> onTriggered;
};

class B : public UTObject {
public:
    void triggerHandler() { ++counter; }
};

int main(int argc, char *argv[]) {
    auto a = new A();
    auto b = new B();

    a->fireEvent();
    a->onTriggered.addEventHandler(b, &B::triggerHandler);
    a->fireEvent();
    a->onTriggered.removeEventHandler(b, &B::triggerHandler);
    delete b;
    a->fireEvent();

    if (counter != 1) {
        return 1;
    }
    
    delete a;
    return 0;
}