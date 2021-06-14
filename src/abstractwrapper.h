#ifndef HLK_ABSTRACT_WRAPPER_H
#define HLK_ABSTRACT_WRAPPER_H

namespace Hlk {

template<class TReturn, class... TParams>
class AbstractWrapper {
    using TWrapper = AbstractWrapper<TReturn, TParams...>;
public:
    virtual ~AbstractWrapper() { }
    virtual TReturn call(TParams...) = 0;

    bool operator==(const TWrapper &other) const {
        return isEquals(other);
    }

    bool operator!=(const TWrapper &other) const { 
        return !(*this == other);
    }

protected:
    virtual bool isEquals(const TWrapper &other) const = 0;
};

} // namespace Hlk

#endif // HLK_ABSTRACT_WRAPPER_H