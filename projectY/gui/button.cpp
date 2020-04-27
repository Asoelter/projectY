#include "button.h"

namespace gui
{

Button::Button(Descriptor desc)
    : descriptor_(desc)
    , hwnd_(NULL)
    , id_(nextID_++)
{
}

Button::~Button()
{
    if (hwnd_)
    {
        DestroyWindow(hwnd_);
    }
}

void Button::attachTo(HWND hwnd)
{
    const auto& [x, y, width, height, title] = descriptor_;
    const auto ctitle = title.c_str();
    const auto style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
    const auto hInstance = GetModuleHandle(NULL);

    const auto button = CreateWindow("button", ctitle, style, x, y, width,
        height, hwnd, reinterpret_cast<HMENU>(id_), hInstance, nullptr);
}

size_t Button::id() const noexcept
{
    return id_;
}

}
