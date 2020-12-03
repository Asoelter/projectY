#include "rectangle.h"

#include <util/ywin.h>

MATH_NAMESPACE_BEGIN

Rectangle::Rectangle(size_t _x, size_t _y, size_t _width, size_t _height)
    : x(_x)
    , y(_y)
    , width(_width)
    , height(_height)
{
    
}

Rectangle Rectangle::defaultPosition(size_t width, size_t height)
{
    return Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, width, height);
}

MATH_NAMESPACE_END
