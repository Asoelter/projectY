#include <algorithm>

template<typename ...Args>
void Signal<Args...>::connect(const std::function<void(Args...)>& func)
{
    functions_.emplace_back(func);
}

template<typename ...Args>
void Signal<Args...>::connect(const Slot<Args...>& slot)
{
    functions_.push_back(slot);
}

template<typename ...Args>
void Signal<Args...>::connect(Slot<Args...>&& slot)
{
    functions_.emplace_back(std::move(slot));
}

template<typename ...Args>
void Signal<Args...>::disconnect(const Slot<Args...>& slot)
{
    functions_.erase(std::find(functions_.begin(), functions_.end(), func));
}

template<typename ...Args>
void Signal<Args...>::emit(Args... args)
{
    for (const auto& func : functions_)
    {
        func(args...);
    }
}

