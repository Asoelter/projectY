#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include <containers/signal.h>

#include <util/ywin.h>

namespace gui
{

class MenuDropDown;
class Menu;

class MenuItem
{
public:
    MenuItem(const std::string& text);

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

class MenuDropDown
{
public:
    MenuDropDown(const std::string& text);

    void append(MenuItem&& item);
    [[nodiscard]] bool contains(size_t id);
    [[nodiscard]] MenuItem* find(size_t id);

private:
    HMENU hmenu_;
    std::vector<MenuItem> menuItems_;
    std::string text_;

    friend class Menu;
};

class Menu
{
public:
    Menu();

    void append(MenuDropDown& dropdown);
    [[nodiscard]] bool contains(size_t id);
    [[nodiscard]] MenuItem* find(size_t id);

    HMENU handle() const;

    static Menu Null;
private:
    struct NullInit {};
    Menu(NullInit);
private:
    HMENU hmenu_;
    std::vector<MenuDropDown> dropdowns_;
};

}

#endif //MENU_H