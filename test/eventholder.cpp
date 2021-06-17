#include "eventholder.h"

#include <iostream>

void EventHolder::print() {
    std::cout << "Event holder print method called\n";
    onPrint();
}