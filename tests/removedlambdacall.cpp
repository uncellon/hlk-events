#include <hlk/events/event.h>
#include <hlk/events/notifiableobject.h>
#include <iostream>

using namespace std;

class A {
public:
    Hlk::Event<> onClick;
};

class B : public Hlk::NotifiableObject {

};

int main(int argc, char *argv[]) {
    auto a = new A();
    auto b = new B();
    a->onClick();
    a->onClick.addEventHandler(b, [&b] () {
        cout << "Clicked!\n";
    });
    a->onClick();
    delete b;
    a->onClick();
}