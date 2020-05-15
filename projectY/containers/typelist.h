#ifndef CONTAINER_TRAITS_H
#define CONTAINER_TRAITS_H

#include <type_traits>

template<typename ...Args>
struct TypeList
{
};

template<typename List>
struct TailT;

template<typename List>
struct HeadT;

template<typename List>
struct IsListT : std::false_type {};

template<typename ...Args>
struct IsListT<TypeList<Args...>> : std::true_type {};

template<typename Head, typename ...Tail>
struct TailT<TypeList<Head, Tail...>>
{
    using type = TypeList<Tail...>;
};

template<typename Head, typename ...Tail>
struct HeadT<TypeList<Head, Tail...>>
{
    using type = Head;
};

template<typename LhsList, typename RhsList>
struct AreEquatableT;

template<typename T, typename U>
struct AreEquatableT<TypeList<T>, TypeList<U>>
{
    static constexpr bool value = std::is_convertible_v<T, U>;
};

template<typename LhsList, typename RhsList>
struct AreEquatableT
{
    static_assert(IsListT<LhsList>::value && IsListT<RhsList>::value);
    static constexpr bool value = std::is_convertible_v<
        std::remove_cv_t<std::remove_reference_t<Head<RhsList>>>, 
        std::remove_cv_t<std::remove_reference_t<Head<LhsList>>>
    > && AreEquatableT<Tail<LhsList>, Tail<RhsList>>::value;
};

template<typename List>
using Tail = typename TailT<List>::type;

template<typename List>
using Head = typename HeadT<List>::type;

template<typename List>
using IsList = typename IsListT<List>::type;

template<typename LhsList, typename RhsList>
constexpr auto AreEquatable = AreEquatableT<LhsList, RhsList>::value;


#endif //CONTAINER_TRAITS_H