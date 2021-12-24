#pragma once

#include <AH/Hardware/Button.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 *
 * @see     Button
 */
template <class Sender, uint8_t NumButtons>
class MIDIButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(const Array<AH::Button, NumButtons> &buttons,
                MIDIAddress baseAddress, RelativeMIDIAddress incrementAddress,
                const Sender &sender)
        : buttons(buttons), baseAddress(baseAddress),
          incrementAddress(incrementAddress), sender(sender) {}

  public:
    void begin() final override {
        for (auto &button : buttons)
            button.begin();
    }
    void update() final override {
        MIDIAddress address = baseAddress;
        for (auto &button : buttons) {
            AH::Button::State state = button.update();
            if (state == AH::Button::Falling) {
                sender.sendOn(address);
            } else if (state == AH::Button::Rising) {
                sender.sendOff(address);
            }
            address += incrementAddress;
        }
    }

    AH::Button::State getButtonState(size_t index) const {
        return buttons[index].getState();
    }

    /// @see @ref AH::Button::invert()
    void invert() {
        for (auto &button : buttons)
            button.invert();
    }

  private:
    Array<AH::Button, NumButtons> buttons;
    const MIDIAddress baseAddress;
    const RelativeMIDIAddress incrementAddress;

  public:
    Sender sender;
};

END_CS_NAMESPACE