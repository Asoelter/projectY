#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

#include "button.h"

#include <util/ywin.h>

namespace gui
{
class Window
{
public:
    Window(size_t width, size_t height, const std::string& title);
    ~Window();

    //TODO(asoelter): consider a different name like "isProcessingMessages"
    [[nodiscard]]
    bool open()  noexcept;
    void update() const noexcept;

    void attach(Button&& button) noexcept;

    LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    [[nodiscard]]
    size_t width() const noexcept;

    [[nodiscard]]
    size_t height() const noexcept;

    [[nodiscard]]
    const HWND & handle() const noexcept;

private:
    static LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    WNDCLASS            wndClass_;
    HWND                hwnd_;
    MSG                 msg_;
    size_t              width_;
    size_t              height_;
    std::string         title_;
    std::vector<Button> buttons_;
    bool                open_;
};
}

#endif //WINDOW_H