#include "menu.h"
#include "menu.h"
#include "menu.h"

#include <algorithm>

#include <util/id_generator.h>
#include <util/type_id.h>

namespace gui
{
    
const MenuItem MenuItem::seperator("separator", MF_SEPARATOR);

MenuItem::MenuItem(const std::string& text)
    : GuiElement(0, 0, 0, 0, text, this)
    , selected()
    , flags_(MF_STRING)
    , id_(GlobalIdGenerator::generate())
    , text_(text)
{
}

size_t MenuItem::typeId() const noexcept
{
    return TypeId<MenuItem>;
}

MenuItem::MenuItem(const char* text, UINT flag)
    : GuiElement(0, 0, 0, 0, text, this)
    , selected()
    , flags_(flag)
    , id_(GlobalIdGenerator::generate())
    , text_(text == NULL ? "" : text)
{
}

MenuDropDown::MenuDropDown(const std::string& text)
    : GuiElement(0, 0, 0, 0, text, this)
    , hmenu_(CreateMenu())
    , menuItems_()
    , text_(text)
{
}

void MenuDropDown::append(MenuItem&& item)
{
    AppendMenu(hmenu_, item.flags_, item.id_, item.text_.c_str());
    menuItems_.emplace_back(std::move(item));

    auto* lastItem = &menuItems_.back();
    elements_[lastItem->name()] = lastItem;
}

bool MenuDropDown::contains(MenuItem::Id id) const
{
    auto predicate = [id](const MenuItem& item) {return item.id_ == id; };

    auto it = std::find_if(menuItems_.begin(), menuItems_.end(), predicate);

    return it == menuItems_.end();
}

MenuItem* MenuDropDown::find(MenuItem::Id id)
{
    auto containsId = [id](const MenuItem& item) {return item.id_ == id; };

    auto it = std::find_if(menuItems_.begin(), menuItems_.end(), containsId);

    if (it == menuItems_.end())
    {
        return nullptr;
    }

    return &*it;
}

size_t MenuDropDown::typeId() const noexcept
{
    return TypeId<MenuDropDown>;
}

Menu Menu::Null{ Menu::NullInit{} };

Menu::Menu(const std::string& name)
    : GuiElement(0u, 0u, 0u, 0u, name, this)
    , hmenu_(CreateMenu())
    , dropdowns_()
{
}

void Menu::append(MenuDropDown&& dropdown)
{
    AppendMenu(hmenu_, MF_POPUP, reinterpret_cast<UINT_PTR>(dropdown.hmenu_), dropdown.text_.c_str());
    dropdowns_.push_back(std::move(dropdown));
    auto* lastDropdown = &dropdowns_.back();
    elements_[lastDropdown->name()] = lastDropdown;
    return;
}

bool Menu::contains(MenuItem::Id id) const noexcept
{
    for (const auto& dropdown : dropdowns_)
    {
        if (dropdown.contains(id))
        {
            return true;
        }
    }

    return false;
}

MenuItem* Menu::find(MenuItem::Id id) noexcept
{
    for (auto& dropdown : dropdowns_)
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

size_t Menu::typeId() const noexcept
{
    return TypeId<Menu>;
}

Menu::Menu(NullInit)
    : GuiElement(0u, 0u, 0u, 0u, "separator", this)
    , hmenu_(NULL)
{
}

}
