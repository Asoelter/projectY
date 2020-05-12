#include <algorithm>

template<typename ...Args>
void Signal<Args...>::connect(const std::function<void(Args...)>& func)
{
    functions_.emplace_back(func);
}

template<typename ...Args>
void Signal<Args...>::connect(const Slot<Args...>& func)
{
    functions_.push_back(func);
}

template<typename ...Args>
void Signal<Args...>::disconnect(const Slot<Args...>& func)
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

