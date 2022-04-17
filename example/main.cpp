#include "hlk/events/event.h"
#include "hlk/events/delegate.h"
#include "hlk/events/notifiableobject.h"
#include "hlk/events/eventloop.h"
#include "hlk/events/object.h"

#include <iostream>
#include <string>
#include <unistd.h>

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

class DerivedObject1 : public Hlk::Object {
public:
    DerivedObject1(Hlk::EventLoop *loop = nullptr) : Hlk::Object(loop) { }

    Hlk::Event<const std::string &> onMessage;
};

class DerivedObject2 : public Hlk::Object {
public:
    DerivedObject2(Hlk::EventLoop *loop = nullptr) : Hlk::Object(loop) { }

    void messageHandler(const std::string &a) {
        std::cout << "Message received: " << a << std::endl;
    }
};

int main(int argc, char *argv[]) {
    // Hlk::Delegate<void(const std::string &)> nd1, nd2;
    // nd1.bind(printFunction);
    // nd1("Calling first delegate");


    // MethodHolder mh;
    // nd1.bind(&mh, &MethodHolder::printMethod);
    // nd1("Calling first delegate");

    // nd1.bind([&mh] (const std::string &message) {
    //     std::cout << "Lambda called: " << message << std::endl;
    // });
    // nd1("Calling first delegate");

    // nd2 = nd1;
    // nd2("Calling second delegate");

    // EventHolder eh;
    // eh.onCall.addEventHandler([] (const std::string &message) {
    //     std::cout << "Lambda called: " << message << std::endl;
    // });
    // eh.onCall.addEventHandler(printFunction);
    // eh.onCall.addEventHandler(&mh, &MethodHolder::printMethod);

    // eh.onCall("Calling onCall(...) event");

    // eh.onCall.removeEventHandler(printFunction);
    // eh.onCall.removeEventHandler(&mh, &MethodHolder::printMethod);

    // eh.onCall("Calling onCall(...) event after removed some event handlers");




    Hlk::EventLoop mainLoop;
    Hlk::EventLoop secondLoop;
    auto derivedObject1 = new DerivedObject1(&mainLoop);
    auto derivedObject2 = new DerivedObject2(&secondLoop);

    derivedObject1->onMessage.addEventHandler(derivedObject2, &DerivedObject2::messageHandler);
    derivedObject1->onMessage("Hello!");

    delete derivedObject2;

    derivedObject1->onMessage("Hey!");

    delete derivedObject1;

    sleep(1000);

    return 0;
}