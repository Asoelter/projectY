namespace gui
{
template<typename ...Args>
void Signal<Args...>::connect(const std::function<void(Args...)>& func)
{
    functions_.push_back(func);
}

template<typename ...Args>
void Signal<Args...>::emit(Args... args)
{
    for (const auto& func : functions_)
    {
        func(args...);
    }
}

}
