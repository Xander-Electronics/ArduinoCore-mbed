#pragma once

#include <AH/Hardware/ButtonMatrix.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     AH::ButtonMatrix
 */
template <class Sender, uint8_t NumRows, uint8_t NumCols>
class MIDIButtonMatrix
    : public MIDIOutputElement,
      public AH::ButtonMatrix<MIDIButtonMatrix<Sender, NumRows, NumCols>,
                              NumRows, NumCols> {
    using ButtonMatrix = AH::ButtonMatrix<MIDIButtonMatrix, NumRows, NumCols>;
    friend class AH::ButtonMatrix<MIDIButtonMatrix, NumRows, NumCols>;

  protected:
    /**
     * @brief   Construct a new MIDIButtonMatrix.
     * 
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the internal 
     *          pull-up resistor will be enabled.
     * @param   addresses
     *          A matrix containing the address corresponding to each button.
     * @param   channelCN
     *          The MIDI channel and optional cable number for all buttons.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonMatrix(const PinList<NumRows> &rowPins,
                     const PinList<NumCols> &colPins,
                     const AddressMatrix<NumRows, NumCols> &addresses,
                     MIDIChannelCable channelCN, const Sender &sender)
        : ButtonMatrix(rowPins, colPins), addresses(addresses),
          baseChannelCN(channelCN), sender(sender) {}

  public:
    void begin() final override { ButtonMatrix::begin(); }

    void update() final override { ButtonMatrix::update(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) {
        int8_t address = addresses[row][col];
        if (state == LOW)
            sender.sendOn({address, baseChannelCN});
        else
            sender.sendOff({address, baseChannelCN});
    }

    AddressMatrix<NumRows, NumCols> addresses;
    MIDIChannelCable baseChannelCN;

  public:
    Sender sender;
};

END_CS_NAMESPACE