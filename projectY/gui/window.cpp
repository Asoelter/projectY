#include "window.h"
#include "window.h"

namespace gui
{
Window::Window(size_t width, size_t height, const std::string& title)
    : wndClass_({ 0 })
    , hwnd_(0)
    , msg_({ 0 })
    , width_(width)
    , height_(height)
    , title_(title)
    , buttons_()
    , open_(true)
{
    const auto hInstance = GetModuleHandle(nullptr);

    wndClass_.style = CS_HREDRAW | CS_VREDRAW;
    wndClass_.lpfnWndProc = WndProcSetup;
    wndClass_.cbClsExtra = NULL;
    wndClass_.cbWndExtra = NULL;
    wndClass_.hInstance = hInstance;
    wndClass_.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass_.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass_.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
    wndClass_.lpszMenuName = nullptr;
    wndClass_.lpszClassName = title.c_str();

    RegisterClass(&wndClass_);

    hwnd_ = CreateWindow(title.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<int>(width_),
        static_cast<int>(height_),
        nullptr,
        nullptr,
        hInstance,
        this);

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

Window::~Window()
{
    DestroyWindow(hwnd_);
}

[[nodiscard]]
bool Window::open() noexcept
{
    return GetMessage(&msg_, hwnd_, 0, 0);
}

void Window::update() const noexcept
{
    TranslateMessage(&msg_);
    DispatchMessage(&msg_);
}

void Window::attach(Button&& button) noexcept
{
    button.attachTo(hwnd_);

    buttons_.push_back(button);
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
        case WM_SIZE:
        {
            width_ = LOWORD(lParam);
            height_ = HIWORD(lParam);
            return 0;
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            auto hdc = BeginPaint(hwnd, &ps);
            auto widthStr = std::to_string(width_);
            auto heightStr = std::to_string(height_);
            auto msg = "width: " + widthStr + " height: " + heightStr;
            TextOut(hdc, 5, 5, msg.c_str(), static_cast<int>(msg.size()));
            EndPaint(hwnd, &ps);
            return 0;
        }break;
        case WM_COMMAND:
        {
            for (auto button : buttons_)
            {
                if (LOWORD(wParam) == button.id())
                {
                    button.pushed.emit();
                }
            }

            return 0;
        }break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

size_t Window::width() const noexcept
{
    return width_;
}

size_t Window::height() const noexcept
{
    return height_;
}

const HWND & Window::handle() const noexcept
{
    return hwnd_;
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

}
