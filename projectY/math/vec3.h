#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <type_traits>

#include <DirectXMath.h>

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

    constexpr vec3 operator+(const vec3& rhs) const noexcept
    {
        return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    constexpr vec3 operator-(const vec3& rhs) const noexcept
    {
        return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    constexpr void operator+=(const vec3& rhs) const noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    constexpr void operator-=(const vec3& rhs) const noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }

    constexpr DirectX::XMVECTOR toXMVector() const noexcept
    {
        return DirectX::XMVectorSet(x, y, z, 1.0f);
    }

    static constexpr auto size = 3;

    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };

        T data[size];
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

template<typename T>
constexpr auto normalize(const math::vec3<T>& v)
{
    const auto magnitude = static_cast<T>(std::sqrt(dot(v, v)));
    return vec3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
}

#endif //VEC_H