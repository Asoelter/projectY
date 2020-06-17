#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <limits>
#include <string>

#include <util/non_owning_ptr.h>
#include <util/type_id.h>
#include <util/ywin.h>

namespace gui
{

class GuiElement
{
public:
    template<typename T>
    GuiElement(UINT x, UINT y, UINT width, UINT height, const std::string& name, NonOwningPtr<T> forDeduction)
        : xPos_(x)
        , yPos_(y)
        , width_(width)
        , height_(height)
        , name_(name)
        , typeId_(TypeId<T>)
    {

    }

    GuiElement(const GuiElement& rhs) = delete;

    GuiElement(GuiElement&& rhs)
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

    virtual ~GuiElement() = default;

    GuiElement& operator=(const GuiElement& rhs) = delete;

    void operator=(GuiElement&& rhs)
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

    [[nodiscard]]
    virtual NonOwningPtr<GuiElement> element(const std::string& name)
    {
        if (elements_.find(name) == elements_.end())
        {
            return nullptr;
        }

        auto result = elements_[name];
        return result;
    }

    template<typename T>
    [[nodiscard]]
    NonOwningPtr<T> elementAs(const std::string& name)
    {
        auto rawResult = element(name);

        if (!rawResult || rawResult->typeId() != TypeId<T>)
        {
            return nullptr;
        }

        auto castedResult = dynamic_cast<T*>(rawResult);
        return castedResult;
    }

    static constexpr auto undefinedMethod = std::numeric_limits<UINT>::max();

protected:
    using ElementPtr = NonOwningPtr<GuiElement>;
    using ElementMap = std::unordered_map<std::string, ElementPtr>;

    UINT        xPos_;
    UINT        yPos_;
    UINT        width_;
    UINT        height_;
    std::string name_;
    size_t      typeId_;
    ElementMap  elements_;
};

}
#endif //GUI_ELEMENT_H