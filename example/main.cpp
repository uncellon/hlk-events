#include "eventholder.h"
#include "eventhandler.h"

#include <iostream>
#include <hlk/events/delegate.h>

void printFunction();

int main(int argc, char *argv[]) {
    std::cout << "=================\nHlk::Events tests\n=================\n";
    
    std::cout << "\nTest delegate with function binding:\n\n";
    Hlk::Delegate<void> functionDelegate;
    functionDelegate.bind(&printFunction);
    functionDelegate();

    // Copy delegate
    auto functionDelegateCopy = functionDelegate;
    std::cout << "Copied delegate calling: ";
    functionDelegateCopy();

    // Move delegate
    auto functionDelegateMoved = std::move(functionDelegate);
    std::cout << "Moved delegate calling: ";
    functionDelegateMoved();

    // Original delegate calling
    std::cout << "Original delegate calling: \n";
    functionDelegate();

    std::cout << "\nTest event handler and event holder classes:\n\n";

    std::cout << "Below 2 messages should be printed:\n\n";

    EventHolder eventHolder;
    EventHandler eventHandler;
    
    eventHolder.onPrint.addEventHandler(&eventHandler, &EventHandler::printHandler);

    // Fire event
    eventHolder.print();

    std::cout << "\nBelow 1 message should be printed:\n\n";

    EventHolder eventHolder2;
    {
        EventHandler eventHandler2;    
        eventHolder2.onPrint.addEventHandler(&eventHandler2, &EventHandler::printHandler);
    }

    // Fire event
    eventHolder2.print();

    return 0;
}

void printFunction() {
    std::cout << "Printing from function\n";
}