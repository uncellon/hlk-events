#include <hlk/events/event.h>
#include <hlk/events/notifiableobject.h>
#include <iostream>

using namespace Hlk;
using namespace std;

unsigned int counter = 0;

class A {
public:
    void fireEvent() { onTriggered(); }
    
    Event<> onTriggered;
};

class B : public NotifiableObject {
public:
    void triggerHandler() { ++counter; }
};

int main(int argc, char *argv[]) {
    auto a = new A();
    auto b = new B();

    a->fireEvent();
    a->onTriggered.addEventHandler(b, &B::triggerHandler);
    a->fireEvent();
    delete b;
    a->fireEvent();

    if (counter != 1) {
        return 1;
    }
    return 0;
}