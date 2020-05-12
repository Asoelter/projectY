#include "keyboard.h"

#include <util\ywin.h>

namespace gui
{
Signal<> Keyboard::backKeyPressed{};
Signal<> Keyboard::tabKeyPressed{};
Signal<> Keyboard::enterKeyPressed{};
Signal<> Keyboard::shiftKeyPressed{};
Signal<> Keyboard::controlKeyPressed{};
Signal<> Keyboard::altKeyPressed{};
Signal<> Keyboard::capsKeyPressed{};
Signal<> Keyboard::escapeKeyPressed{};

bool Keyboard::initialized = false;
Keyboard Keyboard::kbd{};
std::array<Signal<>*, 256> Keyboard::signals;

Keyboard::Keyboard()
{
    if (!initialized)
    {
        initialized = true;

        for (auto& signal : signals)
        {
            signal = nullptr;
        }

        signals[VK_BACK] = &backKeyPressed;
        signals[VK_TAB] = &tabKeyPressed;
        signals[VK_RETURN] = &enterKeyPressed;
        signals[VK_SHIFT] = &shiftKeyPressed;
        signals[VK_CONTROL] = &controlKeyPressed;
        signals[VK_MENU] = &altKeyPressed;
        signals[VK_CAPITAL] = &capsKeyPressed;
        signals[VK_ESCAPE] = &escapeKeyPressed;
    }
}
}
