#include "keyboard.h"

#include <util\ywin.h>

#include "namespace.h"

GUI_NAMESPACE_BEGIN

Signal<> Keyboard::backKeyPressed{};
Signal<> Keyboard::tabKeyPressed{};
Signal<> Keyboard::enterKeyPressed{};
Signal<> Keyboard::shiftKeyPressed{};
Signal<> Keyboard::controlKeyPressed{};
Signal<> Keyboard::altKeyPressed{};
Signal<> Keyboard::capsKeyPressed{};
Signal<> Keyboard::escapeKeyPressed{};
Signal<> Keyboard::spacebarPressed{};
Signal<> Keyboard::leftArrowKeyPressed{};
Signal<> Keyboard::upArrowKeyPressed{};
Signal<> Keyboard::rightArrowKeyPressed{};
Signal<> Keyboard::downArrowKeyPressed{};
Signal<> Keyboard::insertKeyPressed{};
Signal<> Keyboard::deleteKeyPressed{};
Signal<> Keyboard::zeroKeyPressed{};
Signal<> Keyboard::oneKeyPressed{};
Signal<> Keyboard::twoKeyPressed{};
Signal<> Keyboard::threeKeyPressed{};
Signal<> Keyboard::fourKeyPressed{};
Signal<> Keyboard::fiveKeyPressed{};
Signal<> Keyboard::sixKeyPressed{};
Signal<> Keyboard::sevenKeyPressed{};
Signal<> Keyboard::eightKeyPressed{};
Signal<> Keyboard::nineKeyPressed{};
Signal<> Keyboard::aKeyPressed{};
Signal<> Keyboard::bKeyPressed{};
Signal<> Keyboard::cKeyPressed{};
Signal<> Keyboard::dKeyPressed{};
Signal<> Keyboard::eKeyPressed{};
Signal<> Keyboard::fKeyPressed{};
Signal<> Keyboard::gKeyPressed{};
Signal<> Keyboard::hKeyPressed{};
Signal<> Keyboard::iKeyPressed{};
Signal<> Keyboard::jKeyPressed{};
Signal<> Keyboard::kKeyPressed{};
Signal<> Keyboard::lKeyPressed{};
Signal<> Keyboard::mKeyPressed{};
Signal<> Keyboard::nKeyPressed{};
Signal<> Keyboard::oKeyPressed{};
Signal<> Keyboard::pKeyPressed{};
Signal<> Keyboard::qKeyPressed{};
Signal<> Keyboard::rKeyPressed{};
Signal<> Keyboard::sKeyPressed{};
Signal<> Keyboard::tKeyPressed{};
Signal<> Keyboard::uKeyPressed{};
Signal<> Keyboard::vKeyPressed{};
Signal<> Keyboard::wKeyPressed{};
Signal<> Keyboard::xKeyPressed{};
Signal<> Keyboard::yKeyPressed{};
Signal<> Keyboard::zKeyPressed{};
Signal<> Keyboard::numpadZeroKeyPressed{};
Signal<> Keyboard::numpadOneKeyPressed{};
Signal<> Keyboard::numpadTwoKeyPressed{};
Signal<> Keyboard::numpadThreeKeyPressed{};
Signal<> Keyboard::numpadFourKeyPressed{};
Signal<> Keyboard::numpadFiveKeyPressed{};
Signal<> Keyboard::numpadSixKeyPressed{};
Signal<> Keyboard::numpadSevenKeyPressed{};
Signal<> Keyboard::numpadEightKeyPressed{};
Signal<> Keyboard::numpadNineKeyPressed{};

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
        signals[VK_SPACE] = &spacebarPressed;
        signals[VK_LEFT] = &leftArrowKeyPressed;
        signals[VK_UP] = &upArrowKeyPressed;
        signals[VK_RIGHT] = &rightArrowKeyPressed;
        signals[VK_DOWN] = &downArrowKeyPressed;
        signals[VK_INSERT] = &insertKeyPressed;
        signals[VK_DELETE] = &deleteKeyPressed;
        signals[ZeroKey] = &zeroKeyPressed;
        signals[OneKey] = &oneKeyPressed;
        signals[TwoKey] = &twoKeyPressed;
        signals[ThreeKey] = &threeKeyPressed;
        signals[FourKey] = &fourKeyPressed;
        signals[FiveKey] = &fiveKeyPressed;
        signals[SixKey] = &sixKeyPressed;
        signals[SevenKey] = &sevenKeyPressed;
        signals[EightKey] = &eightKeyPressed;
        signals[NineKey] = &nineKeyPressed;
        signals[AKey] = &aKeyPressed;
        signals[BKey] = &bKeyPressed;
        signals[CKey] = &cKeyPressed;
        signals[DKey] = &dKeyPressed;
        signals[EKey] = &eKeyPressed;
        signals[FKey] = &fKeyPressed;
        signals[GKey] = &gKeyPressed;
        signals[HKey] = &hKeyPressed;
        signals[IKey] = &iKeyPressed;
        signals[JKey] = &jKeyPressed;
        signals[KKey] = &kKeyPressed;
        signals[LKey] = &lKeyPressed;
        signals[MKey] = &mKeyPressed;
        signals[NKey] = &nKeyPressed;
        signals[OKey] = &oKeyPressed;
        signals[PKey] = &pKeyPressed;
        signals[QKey] = &qKeyPressed;
        signals[RKey] = &rKeyPressed;
        signals[SKey] = &sKeyPressed;
        signals[TKey] = &tKeyPressed;
        signals[UKey] = &uKeyPressed;
        signals[VKey] = &vKeyPressed;
        signals[WKey] = &wKeyPressed;
        signals[XKey] = &xKeyPressed;
        signals[YKey] = &yKeyPressed;
        signals[ZKey] = &zKeyPressed;
        signals[VK_NUMPAD0] = &numpadZeroKeyPressed;
        signals[VK_NUMPAD1] = &numpadOneKeyPressed;
        signals[VK_NUMPAD2] = &numpadTwoKeyPressed;
        signals[VK_NUMPAD3] = &numpadThreeKeyPressed;
        signals[VK_NUMPAD4] = &numpadFourKeyPressed;
        signals[VK_NUMPAD5] = &numpadFiveKeyPressed;
        signals[VK_NUMPAD6] = &numpadSixKeyPressed;
        signals[VK_NUMPAD7] = &numpadSevenKeyPressed;
        signals[VK_NUMPAD8] = &numpadEightKeyPressed;
        signals[VK_NUMPAD9] = &numpadNineKeyPressed;
    }
}
void Keyboard::emitKey(size_t keyCode)
{
    if (signals[keyCode])
    {
        signals[keyCode]->emit();
    }
}

GUI_NAMESPACE_END
