#ifndef PARENT_WINDOW_H
#define PARENT_WINDOW_H

#include <string>
#include <vector>

#include <math/rectangle.h>

#include <util/ywin.h>


#include "button.h"
#include "namespace.h"

GUI_NAMESPACE_BEGIN

struct ChildTag{};

class ParentWindow
{
public:
    ParentWindow(math::Rectangle outline, const std::string& title);
    ParentWindow(math::Rectangle outline, const std::string& title, ChildTag childTag);

    [[nodiscard]] bool isOpen() const noexcept;
    void update() noexcept;

    [[nodiscard]] size_t width() const noexcept;
    [[nodiscard]] size_t height() const noexcept;

    void attach(Button&& button);

public:
    [[nodiscard]] static size_t typeId() noexcept;

private:
    static LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    WNDCLASSEX createWndClass(const std::string& title);
    HWND createHwnd(const math::Rectangle& rect, UINT style, const std::string& title, HWND parent);
    LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    WNDCLASSEX                  wndClass_;
    HWND                        hwnd_;
    math::Rectangle             outline_;
    std::string                 title_;
    std::vector<Button>         buttons_;
    std::vector<ParentWindow>   windows_;
    bool                        isOpen_;
};

GUI_NAMESPACE_END

#endif //PARENT_WINDOW_H
