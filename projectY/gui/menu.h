#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include <containers/signal.h>

#include <util/ywin.h>

#include "gui_element.h"

#define GUI_ELEMENT_METHODS               \
    [[nodiscard]]                         \
    UINT xPos() const noexcept override   \
    {return GuiElement::undefinedMethod;} \
                                          \
    [[nodiscard]]                         \
    UINT yPos() const noexcept override   \
    {return GuiElement::undefinedMethod;} \
                                          \
    [[nodiscard]]                         \
    UINT width() const noexcept override  \
    {return GuiElement::undefinedMethod;} \
                                          \
    [[nodiscard]]                         \
    UINT height() const noexcept override \
    {return GuiElement::undefinedMethod;} \


namespace gui
{

class MenuDropDown;
class Menu;

class MenuItem : public GuiElement
{
public:
    using Id = size_t;

    MenuItem(const std::string& text);

    GUI_ELEMENT_METHODS

    [[nodiscard]]
    size_t typeId() const noexcept override;

    static const MenuItem seperator;

public: //slots
    Signal<> selected;

private:
    MenuItem(const char* text, UINT flag);
private:
    UINT flags_;
    size_t id_; 
    std::string text_;

    friend class MenuDropDown;
};

class MenuDropDown : public GuiElement
{
public:
    MenuDropDown(const std::string& text);

    GUI_ELEMENT_METHODS

    void append(MenuItem&& item);
    void append(const MenuItem& item);

    [[nodiscard]] 
    bool contains(MenuItem::Id id);

    [[nodiscard]] 
    MenuItem* find(MenuItem::Id id);

    [[nodiscard]]
    size_t typeId() const noexcept override;

private:
    HMENU hmenu_;
    std::vector<MenuItem> menuItems_;
    std::string text_;

    friend class Menu;
};

class Menu : public GuiElement
{
public:
    Menu(const std::string& name = "Menu");

    GUI_ELEMENT_METHODS

    void append(MenuDropDown& dropdown);

    [[nodiscard]] 
    bool contains(MenuItem::Id id) const noexcept;

    [[nodiscard]] 
    MenuItem* find(MenuItem::Id id) noexcept;

    [[nodiscard]]
    HMENU handle() const;

    [[nodiscard]]
    size_t typeId() const noexcept override;

    static Menu Null;

private:
    struct NullInit {};
    Menu(NullInit);

private:
    HMENU                     hmenu_;
    std::vector<MenuDropDown> dropdowns_;
};

}

#ifdef GUI_ELEMENT_METHODS
#   undef GUI_ELEMENT_METHODS
#else
    static_assert(false, "remove me");
#endif

#endif //MENU_H