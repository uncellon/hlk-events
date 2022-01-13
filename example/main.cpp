#include "hlk/events/event.h"
#include "hlk/events/delegate.h"
#include <hlk/events/notifiableobject.h>

#include <iostream>
#include <string>

void printFunction(const std::string &message) {
    std::cout << "Function called: " << message << std::endl;
}

class MethodHolder : public Hlk::NotifiableObject {
public:
    void printMethod(const std::string &message) {
        std::cout << "Method called: " << message << std::endl;
    }
};

class EventHolder {
public:
    Hlk::Event<const std::string &> onCall;
};

int main(int argc, char *argv[]) {
    Hlk::Delegate<void(const std::string &)> nd1, nd2;
    nd1.bind(printFunction);
    nd1("Calling first delegate");


    MethodHolder mh;
    nd1.bind(&mh, &MethodHolder::printMethod);
    nd1("Calling first delegate");

    nd1.bind([&mh] (const std::string &message) {
        std::cout << "Lambda called: " << message << std::endl;
    });
    nd1("Calling first delegate");

    nd2 = nd1;
    nd2("Calling second delegate");

    EventHolder eh;
    eh.onCall.addEventHandler([] (const std::string &message) {
        std::cout << "Lambda called: " << message << std::endl;
    });
    eh.onCall.addEventHandler(printFunction);
    eh.onCall.addEventHandler(&mh, &MethodHolder::printMethod);

    eh.onCall("Calling onCall(...) event");

    eh.onCall.removeEventHandler(printFunction);
    eh.onCall.removeEventHandler(&mh, &MethodHolder::printMethod);

    eh.onCall("Calling onCall(...) event after removed some event handlers");

    return 0;
}