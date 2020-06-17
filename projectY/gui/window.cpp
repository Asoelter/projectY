#include "window.h"

#include "keyboard.h"
#include "mouse.h"

#include <windowsx.h>


namespace gui
{
Window::Window(const WindowRect& rect, const std::string& title, Window* parent)
    : GuiElement(rect.x, rect.y, rect.width, rect.height, title, this)
    , wndClass_({ 0 })
    , hwnd_(0)
    , parent_(parent)
    , children_()
    , buttons_()
    , menu_()
    , open_(true)
{
    const auto hInstance = GetModuleHandle(nullptr);

    wndClass_ = createWndClass();
    RegisterClassEx(&wndClass_);

    auto style = WS_OVERLAPPEDWINDOW | CS_DBLCLKS;

    if (parent)
    {
        style &= 0;
        style |= WS_CHILDWINDOW;
        style |= CS_DBLCLKS;
    }

    HWND parentHandle = NULL;

    if (parent)
    {
        parentHandle = parent->handle();
        parent->attach(this);
    }

    hwnd_ = createHwnd(rect, style, name_, menu_, parentHandle);

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

Window::~Window()
{
    if (hwnd_)
    {
        DestroyWindow(hwnd_);
    }
}

[[nodiscard]]
bool Window::isOpen() const noexcept
{
    return open_;
}

void Window::update() noexcept
{
    MSG msg;
    GetMessage(&msg, hwnd_, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

void Window::attach(Button&& button) noexcept
{
    button.attachTo(hwnd_);

    buttons_.push_back(std::move(button));
    auto& backButton = buttons_.back();
    elements_[backButton.name()] = &backButton;
}

void Window::attach(Menu&& menu) noexcept
{
    menu_ = std::move(menu);
    elements_[menu.name()] = &menu;
    SetMenu(hwnd_, menu.handle());
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
        case WM_MOVE:
        {
            xPos_ = LOWORD(lParam);
            yPos_ = HIWORD(lParam);
        }break;
        case WM_SIZE:
        {
            const auto oldWidth = width_;
            const auto oldHeight = height_;
            width_ = LOWORD(lParam);
            height_ = HIWORD(lParam);

            for (auto elem : elements_)
            {
                if (elem.second && elem.second->typeId() == TypeId<Window>)
                {
                    auto win = static_cast<Window*>(elem.second);
                    const auto horizontalScale = (width_ / static_cast<double>(oldWidth));
                    const auto verticalScale = (height_ / static_cast<double>(oldHeight));
                    const auto x = static_cast<int>(horizontalScale * win->xPos());
                    const auto y = static_cast<int>(verticalScale   * win->yPos());
                    const auto w = static_cast<int>(horizontalScale * win->width());
                    const auto h = static_cast<int>(verticalScale   * win->height());
                    MoveWindow(win->handle(), x, y, w, h, TRUE);
                }
            }
            return 0;
        }break;
        case WM_COMMAND:
        {
            for (auto& button : buttons_)
            {
                if (LOWORD(wParam) == button.id())
                {
                    button.pushed.emit();
                    return 0;
                }
            }

            auto menuItem = menu_.find(LOWORD(wParam));

            if (menuItem)
            {
                emit(menuItem->selected);
            }

            return 0;
        }break;
        case WM_CLOSE:
        {
            open_ = false;
            return 0;
        }break;
        case WM_KEYDOWN:
        {
            Keyboard::emitKey(static_cast<size_t>(wParam));
            return 0;
        }break;
        case WM_MOUSEMOVE:
        {
            const auto x = GET_X_LPARAM(lParam);
            const auto y = GET_Y_LPARAM(lParam);
            emit(Mouse::move, x, y);
            return 0;
        };
        case WM_LBUTTONDOWN:
        {
            const auto x = GET_X_LPARAM(lParam);
            const auto y = GET_Y_LPARAM(lParam);
            emit(Mouse::leftButtonClicked, x, y);
            return 0;
        }break;
        case WM_RBUTTONDOWN:
        {
            const auto x = GET_X_LPARAM(lParam);
            const auto y = GET_Y_LPARAM(lParam);
            emit(Mouse::rightButtonClicked, x, y);
            return 0;
        }break;
        case WM_LBUTTONDBLCLK:
        {
            const auto x = GET_X_LPARAM(lParam);
            const auto y = GET_Y_LPARAM(lParam);
            emit(Mouse::leftButtonDoubleClicked, x, y);
            return 0;
        }break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

UINT Window::width() const noexcept
{
    return width_;
}

UINT Window::height() const noexcept
{
    return height_;
}

const HWND & Window::handle() const noexcept
{
    return hwnd_;
}

size_t Window::typeId() const noexcept
{
    return TypeId<Window>;
}

LRESULT CALLBACK Window::WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (message == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        const auto window = static_cast<Window*>(createStruct->lpCreateParams);

        //store the window with win32 api
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

        //change window's wndproc to forwardMsg
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::forwardMsg));
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return window->WndProc(hwnd, message, wParam, lParam);
}

WNDCLASSEX Window::createWndClass()
{
    WNDCLASSEX wndclass = { 0 };
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProcSetup;
    wndclass.cbClsExtra = NULL;
    wndclass.cbWndExtra = NULL;
    wndclass.hInstance = GetModuleHandle(nullptr);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = name_.c_str();

    return wndclass;
}

HWND Window::createHwnd(const WindowRect& rect, UINT style, const std::string& title, const Menu& menu, HWND parent)
{
    return CreateWindowExA(0, title.c_str(),
        title.c_str(),
        style,
        rect.x,
        rect.y,
        static_cast<int>(rect.width),
        static_cast<int>(rect.height),
        parent,
        menu.handle(),
        GetModuleHandle(nullptr),
        this);
}

void Window::attach(Window* childWindow)
{
    children_.push_back(childWindow);
    elements_[childWindow->name_] = childWindow;
}

}
