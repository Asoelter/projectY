#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <limits>
#include <string>
#include <unordered_map>

#include "../util/pointer.h"
#include "../util/type_id.h"
#include "../util/ywin.h"

#include "namespace.h"

GUI_NAMESPACE_BEGIN

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
    GuiElement(GuiElement&& rhs);
    virtual ~GuiElement() = default;

    GuiElement& operator=(const GuiElement& rhs) = delete;
    void operator=(GuiElement&& rhs);

    [[nodiscard]]
    virtual UINT xPos() const noexcept;

    [[nodiscard]]
    virtual UINT yPos() const noexcept;

    [[nodiscard]]
    virtual UINT width() const noexcept;

    [[nodiscard]]
    virtual UINT height() const noexcept;

    [[nodiscard]]
    virtual std::string name() const noexcept;

    [[nodiscard]]
    virtual size_t typeId() const noexcept = 0;

    [[nodiscard]]
    virtual NonOwningPtr<GuiElement> element(const std::string& name);

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

GUI_NAMESPACE_END

#endif //GUI_ELEMENT_H