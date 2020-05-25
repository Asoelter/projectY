#include "button.h"
#include "button.h"
#include "button.h"

#include <util/id_generator.h>

namespace gui
{

Button::Button(Descriptor desc)
    : GuiElement(desc.x, desc.y, desc.width, desc.height, desc.title, this)
    , hwnd_(NULL)
    , id_(UniqueIdGenerator::generate())
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
    const auto ctitle = name_.c_str();
    const auto style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_FLAT;
    const auto hInstance = GetModuleHandle(NULL);

    const auto button = CreateWindow("button", ctitle, style, xPos_, yPos_, width_,
        height_, hwnd, reinterpret_cast<HMENU>(id_), hInstance, nullptr);
}

size_t Button::id() const noexcept
{
    return id_;
}

std::string Button::name() const noexcept
{
    return name_;
}

size_t Button::typeId() const noexcept
{
    return TypeId<Button>;
}

}
