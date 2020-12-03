#include "parent_window.h"


#include <windowsx.h>
#include <util/type_id.h>

#include "keyboard.h"
#include "mouse.h"

GUI_NAMESPACE_BEGIN

ParentWindow::ParentWindow(math::Rectangle outline, const std::string& title)
    : wndClass_({})
    , hwnd_(nullptr)
    , outline_(outline)
    , title_(title)
    , isOpen_(true)
{
    const auto hInstance = GetModuleHandle(nullptr);

    wndClass_ = createWndClass(title_);
    RegisterClassEx(&wndClass_);

    auto style = WS_OVERLAPPEDWINDOW | CS_DBLCLKS;

    hwnd_ = createHwnd(outline, style, title_, nullptr);

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

ParentWindow::ParentWindow(math::Rectangle outline, const std::string& title, ChildTag childTag)
    : hwnd_(nullptr)
    , wndClass_({})
    , outline_(outline)
    , title_(title)
    , isOpen_(true)
{
    wndClass_ = createWndClass(title_);
    RegisterClassEx(&wndClass_);
    //hwnd_ = createHwnd(outline, WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CHILD | CS_DBLCLKS | WS_VISIBLE, title);
    hwnd_ = createHwnd(outline,  WS_CHILD | CS_DBLCLKS | WS_VISIBLE, title, nullptr);
}

[[nodiscard]]
bool ParentWindow::isOpen() const noexcept
{
    return isOpen_;
}

void ParentWindow::update() noexcept
{
    MSG msg;
    GetMessage(&msg, hwnd_, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

[[nodiscard]]
size_t ParentWindow::width() const noexcept
{
    return outline_.width;
}

[[nodiscard]]
size_t ParentWindow::height() const noexcept
{
    return outline_.height;
}

void ParentWindow::attach(Button&& button)
{
    button.attachTo(hwnd_);
    buttons_.push_back(std::move(button));
}

[[nodiscard]]
size_t ParentWindow::typeId() noexcept
{
    return TypeId<ParentWindow>;
}

LRESULT CALLBACK ParentWindow::WndProcSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (message == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        const auto window = static_cast<ParentWindow*>(createStruct->lpCreateParams);

        //store the window with win32 api
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

        //change window's wndproc to forwardMsg
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&ParentWindow::forwardMsg));
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ParentWindow::forwardMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    const auto window = reinterpret_cast<ParentWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return window->WndProc(hwnd, message, wParam, lParam);
}

WNDCLASSEX ParentWindow::createWndClass(const std::string& title)
{
    WNDCLASSEX wndclass = {};
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProcSetup;
    wndclass.cbClsExtra = NULL;
    wndclass.cbWndExtra = NULL;
    wndclass.hInstance = GetModuleHandle(nullptr);
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = title.c_str();

    return wndclass;
}

LRESULT ParentWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
        case WM_MOVE:
        {
            outline_.x = LOWORD(lParam);
            outline_.y = HIWORD(lParam);
        }break;
        case WM_SIZE:
        {
            outline_.width = LOWORD(lParam);
            outline_.height = HIWORD(lParam);

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
            return 0;
        }break;
        case WM_CLOSE:
        {
            isOpen_ = false;
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
        default: return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

HWND ParentWindow::createHwnd(const math::Rectangle& rect, UINT style, const std::string& title, HWND parent)
{
    return CreateWindowExA(0, title.c_str(),
        title.c_str(),
        style,
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.width),
        static_cast<int>(rect.height),
        parent,
        nullptr,
        GetModuleHandle(nullptr),
        this);
}

GUI_NAMESPACE_END
