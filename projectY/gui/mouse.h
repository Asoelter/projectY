#ifndef MOUSE_H
#define MOUSE_H

#include <containers/signal.h>

namespace gui
{

class Window;

class Mouse
{
public:
    using SignalType = Signal<unsigned short, unsigned short>;
    using SlotType = Slot<unsigned short, unsigned short>;

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