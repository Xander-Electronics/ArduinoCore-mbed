#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatched.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button**, and send out MIDI **Note** events.
 * 
 * It latches the input, so press once to enable, press again to disable 
 * (toggle).
 * 
 * The switch is debounced in software.  
 * This version can be banked.  
 * 
 * @tparam  NumBanks
 *          The number of banks.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <uint8_t NumBanks>
class NoteButtonLatched
    : public MIDIButtonLatched<NumBanks, SingleAddress, DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new bankable NoteButtonLatched object on the given pin 
     *          and with address.
     * 
     * @param   config
     *          The bank to add this element to.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtonLatched(BankConfig<NumBanks> config, pin_t pin,
                      MIDIAddress address, uint8_t velocity = 0x7F)
        : MIDIButtonLatched<NumBanks, SingleAddress, DigitalNoteSender>{
              {config, address},
              pin,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace Bankable

END_CS_NAMESPACE