#include "menu.h"

#include <util/id_generator.h>

namespace gui
{
    
const MenuItem MenuItem::seperator{ NULL, MF_SEPARATOR};

MenuItem::MenuItem(const std::string& text)
    : selected()
    , flags_(MF_STRING)
    , id_(IdGenerator<GloballyUnique>::generate())
    , text_(text)
{
}

MenuItem::MenuItem(const char* text, UINT flag)
    : selected()
    , flags_(flag)
    , id_(IdGenerator<GloballyUnique>::generate())
    , text_(text == NULL ? "" : text)
{
}

MenuDropDown::MenuDropDown(const std::string& text)
    : hmenu_(CreateMenu())
    , menuItems_()
    , text_(text)
{
}

void MenuDropDown::append(MenuItem&& item)
{
    AppendMenu(hmenu_, item.flags_, item.id_, item.text_.c_str());
    menuItems_.push_back(std::move(item));
}

bool MenuDropDown::contains(size_t id)
{
    for (auto item : menuItems_)
    {
        if (item.id_ == id)
        {
            return true;
        }
    }

    return false;
}

MenuItem* MenuDropDown::find(size_t id)
{
    for (auto& item : menuItems_)
    {
        if (item.id_ == id)
        {
            return &item;
        }
    }

    return nullptr;
}

Menu Menu::Null{ Menu::NullInit{} };

Menu::Menu()
    : hmenu_(CreateMenu())
    , dropdowns_()
{
}

void Menu::append(MenuDropDown& dropdown)
{
    AppendMenu(hmenu_, MF_POPUP, reinterpret_cast<UINT>(dropdown.hmenu_), dropdown.text_.c_str());
    dropdowns_.push_back(dropdown);
}

bool Menu::contains(size_t id)
{
    for (auto dropdown : dropdowns_)
    {
        if (dropdown.contains(id))
        {
            return true;
        }
    }

    return false;
}

MenuItem* Menu::find(size_t id)
{
    for (auto dropdown : dropdowns_)
    {
        auto it = dropdown.find(id);

        if (it)
        {
            return it;
        }
    }

    return nullptr;
}

HMENU Menu::handle() const
{
    return hmenu_;
}

Menu::Menu(NullInit)
    : hmenu_(NULL)
{
}

}
