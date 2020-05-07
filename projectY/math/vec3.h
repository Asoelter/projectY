#ifndef VEC_H
#define VEC_H

#include <type_traits>

namespace math
{
template<
        typename T,
        typename = std::enable_if<std::is_arithmetic_v<T>>
        >
struct vec3
{
    constexpr vec3(T _x, T _y, T _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {

    }

    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };

        T data[3];
    };
};
}

template<typename T, typename U>
constexpr auto dot(const math::vec3<T>& lhs, const math::vec3<U>& rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

template<typename T, typename U>
constexpr auto cross(const math::vec3<T>& lhs, const math::vec3<U>& rhs)
{
    const auto firstTerm = (lhs.y * rhs.z) - (rhs.y * lhs.z);
    const auto secondTerm = (lhs.z * rhs.x) - (rhs.z * lhs.x);
    const auto thirdTerm = (lhs.x * rhs.y) - (rhs.x * lhs.y);

    return firstTerm + secondTerm + thirdTerm;
}

#endif //VEC_H