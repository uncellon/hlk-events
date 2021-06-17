#ifndef NOTIFIER_OBJECT_H
#define NOTIFIER_OBJECT_H

#include <memory>
#include <iostream>

namespace Hlk {

class NotifierObject {
public:

    /* 
        Constructor with creating self shared pointer. This is necessary for the 
        delegate to be able to track the destruction of the notifier object 
        using a weak pointer 
    */
    NotifierObject() {
        m_selfSharedPointer = std::make_shared<NotifierObject>(*this);
    }

    // Specifying a friendly class
    template<class TReturn, class... TParams>
    friend class Delegate;

private:
    std::shared_ptr<NotifierObject> m_selfSharedPointer;
};

} // namespace Hlk

#endif //NOTIFIER_OBJECT_H