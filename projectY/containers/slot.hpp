template<typename ...Args>
size_t Slot<Args...>::nextId = 0;

template<typename ...Args>
Slot<Args...>::Slot(const FunctionType& func)
    : function_(func)
    , id_(++nextId)
{

}

template<typename ...Args>
void Slot<Args...>::operator()(Args... args) const
{
    function_(args...);
}

template<typename ...Args>
bool Slot<Args...>::operator==(const Slot& rhs) const
{
    return id_ == rhs.id_;
}

template<typename ...Args>
bool Slot<Args...>::operator!=(const Slot& rhs) const
{
    return !(*this == rhs);
}
