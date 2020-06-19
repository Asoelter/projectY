#include "mouse.h"

GUI_NAMESPACE_BEGIN

Mouse::SignalType Mouse::move{};
Mouse::SignalType Mouse::leftButtonClicked{};
Mouse::SignalType Mouse::rightButtonClicked{};
Mouse::SignalType Mouse::leftButtonDoubleClicked{};

GUI_NAMESPACE_END