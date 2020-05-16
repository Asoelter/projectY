#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

#include <containers/signal.h>

#include <util/ywin.h>

#include "button.h"
#include "menu.h"

namespace gui
{

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

class Window
{
public:
    Window(const WindowRect& rect, const std::string& title, const Menu& menu = Menu::Null, HWND parent = NULL);
    ~Window();

    //TODO(asoelter): consider a different name like "isProcessingMessages"
    [[nodiscard]]
    bool open() const noexcept;
    void update() noexcept;

    void attach(Button&& button) noexcept;

    LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    [[nodiscard]]
    UINT width() const noexcept;

    [[nodiscard]]
    UINT height() const noexcept;

    [[nodiscard]]
    const HWND & handle() const noexcept;

public: //signals
    Signal<> keyPress;

private:
    static LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    WNDCLASSEX createWndClass();
    HWND createHwnd(const WindowRect& rect, UINT style, const std::string& title, const Menu& menu, HWND parent);

private:
    WNDCLASSEX          wndClass_;
    HWND                hwnd_;
    UINT                width_;
    UINT                height_;
    std::string         title_;
    std::vector<Button> buttons_;
    Menu                menu_;
    bool                open_;
};
}

#endif //WINDOW_H