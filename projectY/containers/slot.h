#ifndef SLOT_H
#define SLOT_H

#include <functional>

template<typename ...Args>
class Slot
{
public:
    using FunctionType = std::function<void(Args...)>;

    Slot(const FunctionType& func);

    void operator()(Args... args) const;

    bool operator==(const Slot& rhs) const;
    bool operator!=(const Slot& rhs) const;

private:
    static size_t nextId;
private:
    std::function<void(Args...)> function_;
    size_t id_;
};

#include "slot.hpp"

#endif //SLOT_H