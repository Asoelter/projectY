#ifndef MOUSE_H
#define MOUSE_H

#include <containers/signal.h>

namespace gui
{

class Window;

class Mouse
{
public:
    using SignalType = Signal<int, int>;
    using SlotType = Slot<int, int>;

    static SignalType move;
    static SignalType leftButtonClicked;
    static SignalType rightButtonClicked;
    static SignalType leftButtonDoubleClicked;

private:
    Mouse() = default;

    static Mouse mouse;

    friend class Window;
};

}

#endif //MOUSE_H