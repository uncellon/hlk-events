#define HLK_EVENTS_DEBUG
#include "delegate.h"

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
}

void passDelegate() {
    auto delegate = [] () { std::cout << "4\n"; };
    holder.setDelegate(delegate);
}
