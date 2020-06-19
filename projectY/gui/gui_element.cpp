#include "gui_element.h"

GUI_NAMESPACE_BEGIN

GuiElement::GuiElement(GuiElement&& rhs)
{
    xPos_ = rhs.xPos_;
    yPos_ = rhs.yPos_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    name_ = std::move(rhs.name_);
    typeId_ = rhs.typeId();
    elements_ = std::move(rhs.elements_);

    rhs.xPos_ = 0;
    rhs.yPos_ = 0;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.name_.clear();
    rhs.elements_.clear();
}

void GuiElement::operator=(GuiElement&& rhs)
{
    xPos_ = rhs.xPos_;
    yPos_ = rhs.yPos_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    name_ = std::move(rhs.name_);
    typeId_ = rhs.typeId();
    elements_ = std::move(rhs.elements_);

    rhs.xPos_ = 0;
    rhs.yPos_ = 0;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.name_.clear();
    rhs.elements_.clear();
}

[[nodiscard]]
UINT GuiElement::xPos() const noexcept 
{ 
    return xPos_; 
}

[[nodiscard]]
UINT GuiElement::yPos() const noexcept 
{ 
    return yPos_; 
}

[[nodiscard]]
UINT GuiElement::width() const noexcept 
{ 
    return width_; 
}

[[nodiscard]]
UINT GuiElement::height() const noexcept 
{ 
    return height_; 
}

[[nodiscard]]
std::string GuiElement::name() const noexcept 
{ 
    return name_; 
}

[[nodiscard]]
NonOwningPtr<GuiElement> GuiElement::element(const std::string& name)
{
    if (elements_.find(name) == elements_.end())
    {
        return nullptr;
    }

    auto result = elements_[name];
    return result;
}

GUI_NAMESPACE_END
