#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <type_traits>

template<typename T>
struct RemoveCVRefT
{
    using type = std::remove_cv_t<std::remove_reference<T>>;
};

template<typename T>
using RemoveCVRef = typename RemoveCVRefT<T>::type;

#endif //TYPE_TRAITS_H
