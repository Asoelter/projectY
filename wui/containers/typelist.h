#ifndef CONTAINER_TRAITS_H
#define CONTAINER_TRAITS_H

#include <type_traits>
#include "../util/type_traits.h"

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

template<typename LhsList, typename RhsList>
struct AreSimilarT;

template<typename List, typename T>
struct ContainsT;

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

template<>
struct AreSimilarT<TypeList<>, TypeList<>>
{
    static constexpr bool value = true;
};

template<typename T>
struct AreSimilarT<TypeList<>, TypeList<T>>
{
    static constexpr bool value = false;
};

template<typename T>
struct AreSimilarT<TypeList<T>, TypeList<>>
{
    static constexpr bool value = false;
};

template<typename T, typename U>
struct AreSimilarT<TypeList<T>, TypeList<U>>
{
    static constexpr bool value = std::is_convertible_v<RemoveCVRef<T>, RemoveCVRef<U>>;
};

template<typename LhsList, typename RhsList>
struct AreSimilarT
{
    static_assert(IsListT<LhsList>::value && IsListT<RhsList>::value);

    static constexpr bool value = std::is_convertible_v<
        RemoveCVRef<Head<RhsList>>, 
        RemoveCVRef<Head<LhsList>>
    > && AreSimilarT<Tail<LhsList>, Tail<RhsList>>::value;
};

template<typename T>
struct ContainsT<TypeList<>, T>
{
    static constexpr bool value = false;
};

template<typename List, typename T>
struct ContainsT
{
    static_assert(IsListT<List>::value);

    static constexpr bool value = std::is_same_v<RemoveCVRef<HeadT<List>::type>, RemoveCVRef<T>>
        || ContainsT<Tail<List>, T>::value;
};

template<typename List>
using Tail = typename TailT<List>::type;

template<typename List>
using Head = typename HeadT<List>::type;

template<typename List>
using IsList = typename IsListT<List>::type;

template<typename LhsList, typename RhsList>
constexpr auto AreSimilar = AreSimilarT<LhsList, RhsList>::value;

template<typename List, typename T>
constexpr auto Contains = ContainsT<List, T>::value;


#endif //CONTAINER_TRAITS_H