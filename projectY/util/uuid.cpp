#include "uuid.h"

#include <cassert>

#include "ywin.h"

Uuid::Uuid()
    : value_()
{
    if (UuidCreate(&value_) != RPC_S_OK)
    {
        //TODO(sober adam): find a good exception for this
        assert(false);
    }
}

bool Uuid::operator==(const Uuid& other)
{
    return static_cast<bool>(UuidEqual(this, &other, nullptr));
}

bool Uuid::operator!=(const Uuid& other)
{
    return !(*this == other);
}

std::string Uuid::to_string() const
{
    RPC_WSTR wstr;

    UuidToString(&value_, &wstr);

    return std::to_string(wstr);
}

Uuid Uuid::from_string(const std::string& str) const noexcept
{
    RPC_WSTR wstr = str.c_str();

    UuidFromString(wstr, &value_);

    return value_;
}
