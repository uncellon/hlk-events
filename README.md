# Homebrew Libraries Kit - Events

## Design goals

The main goal is to create a simple and straightforward implementation of delegates and events.

## Examples

### Base delegate usage
```cpp
Hlk::Delegate<void(const std::string &)> printer;
printer.bind([] (const std::string &msg) {
    std::cout << msg << std::endl;
});
printer("My message");
```

### Events
```cpp
...

class A {
public:
    Hlk::Event<void()> onTriggered;

    void fireEvent() { onTriggered(); }
}

class B : public Hlk::NotifiableObject {
public:
    void triggeredHandler() { std::cout << "Class A triggered!\n"; }
}

int main(int argc, char *argv[]) {
    A a;
    B b;
    a.onTriggered.addEventHandler(&b, &B::triggeredHandler);
    a.fireEvent(); // After this call, the message "Class A triggered!" will appear in the program output
    return 0;
}

```

## License

<img align="right" src="https://www.gnu.org/graphics/lgplv3-with-text-154x68.png">

The library is licensed under [GNU Lesser General Public License 3.0](https://www.gnu.org/licenses/lgpl-3.0.txt):

Copyright © 2021 Dmitry Plastinin

Hlk Events is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as pubblished by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Hlk Events is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details