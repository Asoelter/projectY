#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <optional>
#include <array>

#include <containers/signal.h>

namespace gui
{

class Window;

class Keyboard
{
public:
    static Signal<> backKeyPressed;
    static Signal<> tabKeyPressed;
    static Signal<> enterKeyPressed;
    static Signal<> shiftKeyPressed;
    static Signal<> controlKeyPressed;
    static Signal<> altKeyPressed;
    static Signal<> capsKeyPressed;
    static Signal<> escapeKeyPressed;

private:
    Keyboard();

    static bool initialized;
    static Keyboard kbd;
    static std::array<Signal<>*, 256> signals;

    friend class Window;
};


}

#endif //KEYBOARD_H
