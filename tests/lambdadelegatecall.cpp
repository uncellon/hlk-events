#include <hlk/events/delegate.h>

using namespace Hlk;

bool result = false;

int main(int argc, char *argv[]) {
    Delegate<void(bool)> delegate;
    delegate.bind([] (bool value) {
        result = value;
    });
    delegate(true);
    if (!result) {
        return 1;
    }
    return 0;
}