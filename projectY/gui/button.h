#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <containers/signal.h>

#include <util/ywin.h>

#include "gui_element.h"

namespace gui
{

class Button : public GuiElement
{
public:
    struct Descriptor
    {
        Descriptor(UINT _x, UINT _y, UINT _w, UINT _h, const std::string& _t = "") 
            : x(_x), y(_y), width(_w), height(_h), title(_t) {}
        UINT x;
        UINT y;
        UINT width;
        UINT height;
        std::string title;
    };
public:
    Button(Descriptor desc);
    ~Button();

    void attachTo(HWND hwnd);

    [[nodiscard]]
    size_t id() const noexcept;

    [[nodiscard]]
    std::string name() const noexcept;

    [[nodiscard]]
    size_t typeId() const noexcept override;

public: //signals
    Signal<> pushed;

private:
    HWND hwnd_;
    size_t id_;
};

}

#endif //BUTTON_H