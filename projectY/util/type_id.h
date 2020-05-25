#ifndef TYPE_ID_H
#define TYPE_ID_H

#include "id_generator.h"

namespace
{
struct LocalToTypeId {};
}

template<typename T>
struct TypeIdGenerator
{
    static auto value()
    {
        static auto value = IdGenerator<LocalToTypeId>::generate();
        return value;
    }

};

template<typename T>
const auto TypeId = TypeIdGenerator<T>::value();

#endif //TYPE_ID_H