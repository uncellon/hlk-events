// #define HLK_EVENTS_DEBUG
#include "delegate.h"
#include "notifierobject.h"
#include "eventholder.h"
#include "eventhandler.h"

#include <iostream>
#include <vector>

class DelegateHolder {
public:
    template<typename U>
    void setDelegate(U&& delegate) {
        delegate_ = delegate;
    }
    Hlk::Delegate<void> delegate_;
};

class Printer {
public:
    Printer() {
        std::cout << "I am created\n";
    }

    ~Printer() {
        std::cout << "I am destroyed\n";
    }

    void print() {
        std::cout << "I am printer!\n";
    }
};

DelegateHolder holder;

void passDelegate();

int main(int argc, char *argv[]) {
    holder.setDelegate([] () { std::cout << "1\n"; });
    holder.delegate_();

    auto delegate = [] () { std::cout << "2\n"; };
    holder.setDelegate(delegate);
    holder.delegate_();

    holder.setDelegate([] () { std::cout << "3\n"; });
    holder.delegate_();

    passDelegate();
    holder.delegate_();

    std::cout << "End\n";


    /* Test class with event and class with event handler */
    std::cout << "Test class with events and class with event handler 1\n";
    EventHolder holder;
    {
        EventHandler handler;
        holder.onPrint.addEventHandler(&handler, &EventHandler::printHandler);
    }
    holder.print();

    std::cout << "Test class with events and class with event handler 2\n";
    EventHolder holder2;
    EventHandler handler2;
    holder2.onPrint.addEventHandler(&handler2, &EventHandler::printHandler);
    holder2.print();

    std::cout << "Test class with events and class with event handler 3\n";
    EventHolder holder3;
    EventHandler handler3;
    holder3.onPrint.addEventHandler([&handler3] () {
        std::cout << "Print from lambda with captured handler 3\n";
    });
    holder3.print();

    std::cout << "Test class with events and class with event handler 4\n";
    EventHolder holder4;
    {
        EventHandler handler4;
        holder4.onPrint.addEventHandler([&handler4] () {
            std::cout << "Print from lambda with captured handler 4\n";
        }, &handler4);
    }
    holder4.print();
}

void passDelegate() {
    auto delegate = [] () { std::cout << "4\n"; };
    holder.setDelegate(delegate);
}
