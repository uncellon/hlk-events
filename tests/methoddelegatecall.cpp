#include <hlk/events/delegate.h>

using namespace Hlk;

class Holder {
public:
    bool getResult() const { return m_result; }
    void setResult(bool value) { m_result = value; }

protected:
    bool m_result = false;
};

int main(int argc, char *argv[]) {
    Holder h;
    Delegate<void(bool)> delegate(&h, &Holder::setResult);
    delegate(true);
    if (!h.getResult()) {
        return 1;
    }
    return 0;
}