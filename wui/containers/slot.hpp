#include "../util/id_generator.h"

namespace
{
struct LocalToSlot {};
}

using FunctionIdGenerator = IdGenerator<LocalToSlot>;

template<typename ...Args>
Slot<Args...>::Slot(const FunctionType& func)
    : function_(func)
    , id_(FunctionIdGenerator::generate())
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
