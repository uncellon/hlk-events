# Homebrew Libraries Kit - Events

- [Description](#description)
- [Prerequisites](#prerequisites)
- [Examples](#examples)
    - [Function delegate](#function-delegate)
    - [Method delegate](#method-delegate)
    - [Lambda delegate](#lambda-delegate)
    - [Event](#event)
- [License](#license)

## Description

The library provides the following structures:
1. Delegate - a wrapper object for functions, methods or lambdas
2. Event - a collection of delegates. The event executes the code of all containing delegates

## Prerequisites

- C++17 and higher
- CMake >= 3.16

## Examples

### Function delegate

```cpp
function myFunction() {
    std::cout << "Print from function\n";
}

Hlk::Delegate<void()> del;
del.bind(myFunction);
del(); // Will print "Print from function" on the console
```

### Method delegate

```cpp
class A {
public:
    void printNumber(int number) {
        std::cout << "Print number from method: " << number << std::endl;
    }
};

A a;

Hlk::Delegate<void(int)> del;
del.bind(&a, &A::printNumber);
del(42); // Will print "Print number from method: 42" on the console
```

### Lambda delegate

```cpp
Hlk::Delegate<int(int, int)> del;
del.bind([] (int a, int b) {
    return a + b;
});

std::cout << "Sum 1 + 3 = " << del(1, 3) << std::endl; // Will print "Sum 1 + 3 = 4" on the console
```

### Event

```cpp
class EventHolder {
public:
    void fireEvent() {
        onSomeDataChanged();
    }

    Event<> onSomeDataChanged;
};

class EventHandler : public Hlk::NotifiableObject {
public:
    void someDataChangedHandler() {
        std::cout << "Some data changed\n";
    }
};

EventHolder eHolder;
EventHandler eHandler1, eHandler2;

eHolder.onSomeDataChanged.addEventHandler(&eHandler1, &EventHandler::someDataChangedHandler);
eHolder.onSomeDataChanged.addEventHandler(&eHandler2, &EventHandler::someDataChangedHandler);

eHolder.fireEvent(); // Will print "Some data changed" on the console twice
```

## License

<img align="right" src="https://www.gnu.org/graphics/lgplv3-with-text-154x68.png">

The library is licensed under [GNU Lesser General Public License 3.0](https://www.gnu.org/licenses/lgpl-3.0.txt):

Copyright © 2021 Dmitry Plastinin

Hlk Events is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as pubblished by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Hlk Events is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details