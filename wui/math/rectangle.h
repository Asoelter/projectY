#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "namespace.h"

MATH_NAMESPACE_BEGIN

struct Rectangle
{
    Rectangle(size_t _x, size_t _y, size_t _width, size_t _height);

    static Rectangle defaultPosition(size_t width, size_t height);

    size_t x;
    size_t y;
    size_t width;
    size_t height;
};

MATH_NAMESPACE_END

#endif //RECTANGLE_H
