#include "uuid.h"

#include <cassert>

#include "ywin.h"

//TODO(asoelter): fix c-style casts

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
    RPC_STATUS discard;
    return static_cast<bool>(UuidEqual(&value_, &(UUID)other.value_, &discard));
}

bool Uuid::operator!=(const Uuid& other)
{
    return !(*this == other);
}

std::string Uuid::to_string() const
{
    RPC_CSTR wstr;

    if (UuidToString(&value_, &wstr) != RPC_S_OK)
    {
        //TODO(sober adam): find a good exception for this
        assert(false);
    }

    return std::string((const char*)wstr);
}

Uuid Uuid::from_string(const std::string& str) const noexcept
{
    RPC_CSTR wstr = (RPC_CSTR)str.c_str();

    if(UuidFromString(wstr, &(UUID)value_) != RPC_S_OK)
    {
        //TODO(sober adam): find a good exception for this
        assert(false);
    }

    return *this;
}
