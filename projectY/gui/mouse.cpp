#include "mouse.h"

namespace gui
{
Mouse::SignalType Mouse::move{};
Mouse::SignalType Mouse::leftButtonClicked{};
Mouse::SignalType Mouse::rightButtonClicked{};
Mouse::SignalType Mouse::leftButtonDoubleClicked{};
}