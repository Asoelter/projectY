#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <string>

#include <util/type_id.h>
#include <util/ywin.h>

namespace gui
{

class GuiElement
{
public:
    [[nodiscard]]
    virtual UINT xPos() const noexcept { return xPos_; }

    [[nodiscard]]
    virtual UINT yPos() const noexcept { return yPos_; }

    [[nodiscard]]
    virtual UINT width() const noexcept { return width_; }

    [[nodiscard]]
    virtual UINT height() const noexcept { return height_; }

    [[nodiscard]]
    virtual std::string name() const noexcept { return name_; }

    [[nodiscard]]
    virtual size_t typeId() const noexcept = 0;

protected:
    template<typename T>
    GuiElement(UINT x, UINT y, UINT width, UINT height, const std::string& name, T* forDeduction)
        : xPos_(x)
        , yPos_(y)
        , width_(width)
        , height_(height)
        , name_(name)
        , typeId_(TypeId<T>)
    {

    }
    virtual ~GuiElement() = default;

    UINT xPos_;
    UINT yPos_;
    UINT width_;
    UINT height_;
    std::string name_;
    size_t typeId_;
};

}
#endif //GUI_ELEMENT_H