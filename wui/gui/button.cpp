#include "button.h"

#include <util/id_generator.h>

GUI_NAMESPACE_BEGIN

Button::Button(Descriptor desc)
    : GuiElement(desc.x, desc.y, desc.width, desc.height, desc.title, this)
    , hwnd_(NULL)
    , id_(GlobalIdGenerator::generate())
{
}

//If this causes trouble in the future
//it should probably call GuiElement's
//move constructor with rhs as a 
//GuiElement
Button::Button(Button&& rhs) noexcept
    : GuiElement(rhs.xPos_, rhs.yPos_, rhs.width_, rhs.height_, rhs.name_, this)
    , hwnd_(rhs.hwnd_)
    , id_(rhs.id_)
{
    elements_ = std::move(rhs.elements_);
    pushed = std::move(rhs.pushed);

    rhs.xPos_ = 0;
    rhs.yPos_ = 0;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.name_.clear();
}

Button::~Button()
{
    if (hwnd_)
    {
        DestroyWindow(hwnd_);
    }
}

void Button::operator=(Button&& rhs) noexcept
{
    xPos_   = rhs.xPos_;
    yPos_   = rhs.yPos_;
    width_  = rhs.width_;
    height_ = rhs.height_;
    name_   = std::move(rhs.name_);
    pushed  = std::move(rhs.pushed);

    rhs.xPos_   = 0;
    rhs.yPos_   = 0;
    rhs.width_  = 0;
    rhs.height_ = 0;
    rhs.name_.clear();
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

GUI_NAMESPACE_END
