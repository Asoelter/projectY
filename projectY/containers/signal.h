#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <vector>

#include "typelist.h"
#include "slot.h"

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
    std::vector<Slot<Args...>> functions_;
};

//Utility functions for similarities with qt
template<typename ...Args>
void connect(Signal<Args...>& signal, Slot<Args...> slot)
{
    signal.connect(slot);
}

template<typename ...Args, typename ...Args2>
void emit(Signal<Args...>& signal, Args2... args)
{
    static_assert(AreEquatable<TypeList<Args...>, TypeList<Args2...>>, 
        "Incompatible arguments passed to signal");

    signal.emit(args...);
}

#include "signal.hpp"

#endif //SIGNAL_H