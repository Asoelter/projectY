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

template<typename List>
using Tail = typename TailT<List>::type;

template<typename List>
using Head = typename HeadT<List>::type;

#endif //CONTAINER_TRAITS_H