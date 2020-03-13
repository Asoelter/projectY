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
        Descriptor() : x(0), y(0), width(0), height(0), title() {}
        int x;
        int y;
        int width;
        int height;
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