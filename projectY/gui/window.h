#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <unordered_map>
#include <vector>

#include <containers/signal.h>

#include <util/type_id.h>
#include <util/ywin.h>

#include "button.h"
#include "gui_element.h"
#include "menu.h"
#include "namespace.h"

GUI_NAMESPACE_BEGIN

struct WindowRect
{
    WindowRect(UINT _x, UINT _y, UINT _w, UINT _h)
        : x(_x)
        , y(_y)
        , width(_w)
        , height(_h)
    {

    }

    WindowRect(UINT _w, UINT _h)
        : x(CW_USEDEFAULT)
        , y(CW_USEDEFAULT)
        , width(_w)
        , height(_h)
    {

    }

    UINT x;
    UINT y;
    UINT width;
    UINT height;
};

class Window : public GuiElement
{
public:
    Window(const WindowRect& rect, const std::string& title, Window* parent = nullptr);
    ~Window();

    [[nodiscard]]
    bool isOpen() const noexcept;
    void update() noexcept;

    void attach(Button&& button) noexcept;
    void attach(Menu&& menu) noexcept;

    LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    [[nodiscard]]
    UINT width() const noexcept;

    [[nodiscard]]
    UINT height() const noexcept;

    [[nodiscard]]
    const HWND & handle() const noexcept;

    [[nodiscard]]
    size_t typeId() const noexcept override;

private:
    static LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    WNDCLASSEX createWndClass();
    HWND createHwnd(const WindowRect& rect, UINT style, const std::string& title, const Menu& menu, HWND parent);
    void attach(Window* childWindow);

private:
    using ElementMap = std::unordered_map<std::string, GuiElement*>;

    WNDCLASSEX           wndClass_;
    HWND                 hwnd_;
    Window*              parent_;
    std::vector<Window*> children_;
    std::vector<Button>  buttons_; 
    ElementMap           elements_;
    Menu                 menu_;
    bool                 open_;
};

GUI_NAMESPACE_END

#endif //WINDOW_H