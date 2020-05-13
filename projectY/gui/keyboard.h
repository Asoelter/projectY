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
    using SignalType = Signal<>;
    using SlotType = Slot<>;

    static Signal<> backKeyPressed;
    static Signal<> tabKeyPressed;
    static Signal<> enterKeyPressed;
    static Signal<> shiftKeyPressed;
    static Signal<> controlKeyPressed;
    static Signal<> altKeyPressed;
    static Signal<> capsKeyPressed;
    static Signal<> escapeKeyPressed;
    static Signal<> spacebarPressed;
    static Signal<> leftArrowKeyPressed;
    static Signal<> upArrowKeyPressed;
    static Signal<> rightArrowKeyPressed;
    static Signal<> downArrowKeyPressed;
    static Signal<> insertKeyPressed;
    static Signal<> deleteKeyPressed;
    static Signal<> zeroKeyPressed;
    static Signal<> oneKeyPressed;
    static Signal<> twoKeyPressed;
    static Signal<> threeKeyPressed;
    static Signal<> fourKeyPressed;
    static Signal<> fiveKeyPressed;
    static Signal<> sixKeyPressed;
    static Signal<> sevenKeyPressed;
    static Signal<> eightKeyPressed;
    static Signal<> nineKeyPressed;
    static Signal<> aKeyPressed;
    static Signal<> bKeyPressed;
    static Signal<> cKeyPressed;
    static Signal<> dKeyPressed;
    static Signal<> eKeyPressed;
    static Signal<> fKeyPressed;
    static Signal<> gKeyPressed;
    static Signal<> hKeyPressed;
    static Signal<> iKeyPressed;
    static Signal<> jKeyPressed;
    static Signal<> kKeyPressed;
    static Signal<> lKeyPressed;
    static Signal<> mKeyPressed;
    static Signal<> nKeyPressed;
    static Signal<> oKeyPressed;
    static Signal<> pKeyPressed;
    static Signal<> qKeyPressed;
    static Signal<> rKeyPressed;
    static Signal<> sKeyPressed;
    static Signal<> tKeyPressed;
    static Signal<> uKeyPressed;
    static Signal<> vKeyPressed;
    static Signal<> wKeyPressed;
    static Signal<> xKeyPressed;
    static Signal<> yKeyPressed;
    static Signal<> zKeyPressed;
    static Signal<> numpadZeroKeyPressed;
    static Signal<> numpadOneKeyPressed;
    static Signal<> numpadTwoKeyPressed;
    static Signal<> numpadThreeKeyPressed;
    static Signal<> numpadFourKeyPressed;
    static Signal<> numpadFiveKeyPressed;
    static Signal<> numpadSixKeyPressed;
    static Signal<> numpadSevenKeyPressed;
    static Signal<> numpadEightKeyPressed;
    static Signal<> numpadNineKeyPressed;

private:
    Keyboard();

    static void emitKey(size_t keyCode);

    static bool initialized;
    static Keyboard kbd;
    static std::array<Signal<>*, 256> signals;

    friend class Window;

    enum KeyCode
    {
        ZeroKey = 0x30, OneKey, TwoKey, ThreeKey,FourKey, FiveKey, 
        SixKey, SevenKey, EightKey, NineKey,

        AKey = 0x41, BKey, CKey, DKey, EKey, FKey, GKey, HKey, IKey,
        JKey, KKey, LKey, MKey, NKey, OKey, PKey, QKey, RKey, SKey, 
        TKey, UKey, VKey, WKey, XKey, YKey, ZKey
    };
};


}

#endif //KEYBOARD_H
