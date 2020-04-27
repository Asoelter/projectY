#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <containers/signal.h>

#include <util/ywin.h>

namespace gui
{

class Button
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

public: //signals
    Signal<> pushed;

private:
    inline static size_t nextID_ = 1;

    Descriptor descriptor_;
    HWND hwnd_;
    size_t id_;
};

}

#endif //BUTTON_H