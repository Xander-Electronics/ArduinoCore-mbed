#pragma once

#include "MIDI_Pipes.hpp"
#include "MIDI_Sender.hpp"
#include "MIDI_Staller.hpp"
#include <AH/Containers/Updatable.hpp>
#include <Def/Def.hpp>
#include <Def/MIDIAddress.hpp>
#include <MIDI_Parsers/MIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

constexpr auto MIDI_BAUD = 31250;

class MIDI_Callbacks;

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class MIDI_Interface : public TrueMIDI_SinkSource,
                       public MIDI_Sender<MIDI_Interface>,
                       public AH::Updatable<MIDI_Interface>,
                       protected MIDIStaller {
  protected:
    MIDI_Interface() = default;
    MIDI_Interface(MIDI_Interface &&) = default;

  public:
    /// Destructor.
    virtual ~MIDI_Interface();

    /// Initialize the MIDI Interface.
    void begin() override {}
    /// Read the MIDI interface and call the callback if a message was received.
    void update() override = 0;

    /// @name   Default MIDI Interfaces
    /// @{

    /// Set this MIDI interface as the default interface.
    void setAsDefault();
    /// Return the default MIDI interface. If the default MIDI interface was
    /// configured explicitly using @ref setAsDefault(), that interface is
    /// returned. If it wasn't set, or if that MIDI interface no longer exists,
    /// this function returns the newest MIDI interface, the one that was
    /// constructed most recently. If no MIDI interfaces exist, `nullptr` is
    /// returned.
    static MIDI_Interface *getDefault();

    /// @}

    /// @name   MIDI Input Callbacks
    /// @{

    /// Set the callbacks that will be called when a MIDI message is received.
    /// @param   cb
    ///          A pointer to the callback object.
    void setCallbacks(MIDI_Callbacks *cb) { this->callbacks = cb; }
    /// Set the callbacks that will be called when a MIDI message is received.
    /// @param   cb
    ///          A reference to the callback object.
    void setCallbacks(MIDI_Callbacks &cb) { setCallbacks(&cb); }

    /// @}

  protected:
    friend class MIDI_Sender<MIDI_Interface>;
    /// Low-level function for sending a MIDI channel voice message.
    virtual void sendChannelMessageImpl(ChannelMessage) = 0;
    /// Low-level function for sending a MIDI system common message.
    virtual void sendSysCommonImpl(SysCommonMessage) = 0;
    /// Low-level function for sending a system exclusive MIDI message.
    virtual void sendSysExImpl(SysExMessage) = 0;
    /// Low-level function for sending a MIDI real-time message.
    virtual void sendRealTimeImpl(RealTimeMessage) = 0;
    /// Low-level function for sending any buffered outgoing MIDI messages.
    virtual void sendNowImpl() = 0;

  protected:
    /// Accept an incoming MIDI Channel message from the source pipe.
    void sinkMIDIfromPipe(ChannelMessage msg) override { send(msg); }
    /// Accept an incoming MIDI System Exclusive message from the source pipe.
    void sinkMIDIfromPipe(SysExMessage msg) override { send(msg); }
    /// Accept an incoming MIDI System Common message from the source pipe.
    void sinkMIDIfromPipe(SysCommonMessage msg) override { send(msg); }
    /// Accept an incoming MIDI Real-Time message from the source pipe.
    void sinkMIDIfromPipe(RealTimeMessage msg) override { send(msg); }

  protected:
    /// Call the channel message callback and send the message to the sink pipe.
    void onChannelMessage(ChannelMessage message);
    /// Call the SysEx message callback and send the message to the sink pipe.
    void onSysExMessage(SysExMessage message);
    /// Call the System Common message callback and send the message to the sink
    /// pipe.
    void onSysCommonMessage(SysCommonMessage message);
    /// Call the real-time message callback and send the message to the sink
    /// pipe.
    void onRealTimeMessage(RealTimeMessage message);

  public:
    /// Read, parse and dispatch incoming MIDI messages on the given interface.
    template <class MIDIInterface_t>
    static void updateIncoming(MIDIInterface_t *iface);
    /// Dispatch the given type of MIDI message from the given interface.
    template <class MIDIInterface_t>
    static void dispatchIncoming(MIDIInterface_t *iface, MIDIReadEvent event);
    /// Un-stall the given MIDI interface. Assumes the interface has been
    /// stalled because of a chunked SysEx messages. Waits untill that message
    /// is finished.
    template <class MIDIInterface_t>
    static void handleStall(MIDIInterface_t *iface);

  private:
    MIDI_Callbacks *callbacks = nullptr;

  private:
    static MIDI_Interface *DefaultMIDI_Interface;
};

template <class MIDIInterface_t>
void MIDI_Interface::updateIncoming(MIDIInterface_t *iface) {
    if (iface->getStaller() == iface)
        iface->unstall(iface);
    bool chunked = false;
    MIDIReadEvent event = iface->read();
    while (event != MIDIReadEvent::NO_MESSAGE) {
        dispatchIncoming(iface, event);
        if (event == MIDIReadEvent::SYSEX_CHUNK)
            chunked = true;
        if (event == MIDIReadEvent::SYSEX_MESSAGE)
            chunked = false;
        event = iface->read();
    }
    if (chunked)
        iface->stall(iface);
    // TODO: add logic to detect MIDI messages such as (N)RPN that span over
    // multiple channel voice messages and that shouldn't be interrupted.
    // For short messages such as (N)RPN, I suggest waiting with a timeout.
}

template <class MIDIInterface_t>
void MIDI_Interface::dispatchIncoming(MIDIInterface_t *iface,
                                      MIDIReadEvent event) {
    switch (event) {
        case MIDIReadEvent::CHANNEL_MESSAGE:
            iface->onChannelMessage(iface->getChannelMessage());
            break;
        case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
        case MIDIReadEvent::SYSEX_MESSAGE:
            iface->onSysExMessage(iface->getSysExMessage());
            break;
        case MIDIReadEvent::SYSCOMMON_MESSAGE:
            iface->onSysCommonMessage(iface->getSysCommonMessage());
            break;
        case MIDIReadEvent::REALTIME_MESSAGE:
            iface->onRealTimeMessage(iface->getRealTimeMessage());
            break;
        case MIDIReadEvent::NO_MESSAGE: break; // LCOV_EXCL_LINE
        default: break;                        // LCOV_EXCL_LINE
    }
}

template <class MIDIInterface_t>
void MIDI_Interface::handleStall(MIDIInterface_t *iface) {
    iface->unstall(iface);

    unsigned long startTime = millis();
    while (millis() - startTime < SYSEX_CHUNK_TIMEOUT) {
        MIDIReadEvent event = iface->read();
        dispatchIncoming(iface, event);
        if (event == MIDIReadEvent::SYSEX_CHUNK)
            startTime = millis(); // reset timeout
        else if (event == MIDIReadEvent::SYSEX_MESSAGE)
            return;
    }
    DEBUGREF(F("Warning: Unable to un-stall pipes: ")
             << iface->getStallerName());
}

END_CS_NAMESPACE
