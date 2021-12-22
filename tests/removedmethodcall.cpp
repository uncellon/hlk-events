#include <hlk/events/event.h>
#include <hlk/events/notifiableobject.h>
#include <iostream>

using namespace std;

class A {
public:
    Hlk::Event<> onClick;
};

class B : public Hlk::NotifiableObject {
public:
    void clickHandler() {
        cout << "Clicked!\n";
    }

    void anotherClickHandler() {
        cout << "Another clicked!\n";
    }
};

int main(int argc, char *argv[]) {
    auto a = new A();
    auto b = new B();
    a->onClick();
    a->onClick.addEventHandler(b, &B::clickHandler);
    a->onClick.addEventHandler(b, &B::anotherClickHandler);
    a->onClick();
    delete b;
    a->onClick();
}