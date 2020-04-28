#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <vector>

#include "slot.h"

namespace gui
{

template<typename ...Args>
class Signal
{
public:
    Signal() = default;

    void connect(const std::function<void(Args...)>& func);
    void connect(const Slot<Args...>& func);
    void disconnect(const Slot<Args...>& func);

    void emit(Args... args);

private:
    //std::vector<std::function<void(Args...)>> functions_;
    std::vector<Slot<Args...>> functions_;
};

}

#include "signal.hpp"

#endif //SIGNAL_H