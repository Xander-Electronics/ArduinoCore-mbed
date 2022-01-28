#include <Control_Surface.h>
#include <APA102.h>

// Define which pins to use.
const uint8_t dataPin = A2;
const uint8_t clockPin = A3;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 16;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];

// Set the brightness to use (the maximum is 31).
const uint8_t brightness = 5;

USBMIDI_Interface midi;             //Init MIDI over USB

CD74HC4067 muxPots = {
  A0,                               //Analog pin
  {MUX1_A, MUX1_B, MUX1_C, MUX1_D}  //Address pins
};

CD74HC4067 muxButtons = {
  A1,                               //Digital pin
  {MUX2_A, MUX2_B, MUX2_C, MUX2_D}  //Address pins
};
/*
   - When the button on pin is pressed, a MIDI Control Change message with a
     value of 0x7F (127) is sent
   - When the button on pin is pressed again, a MIDI Control Change message with a
     value of 0x00 (0) is sent
*/

CCButtonLatched buttons0_15[] = {
  { muxButtons.pin(0),   {17, CHANNEL_1}},
  { muxButtons.pin(1),   {18, CHANNEL_1}},
  { muxButtons.pin(2),   {19, CHANNEL_1}},
  { muxButtons.pin(3),   {20, CHANNEL_1}},
  { muxButtons.pin(4),   {21, CHANNEL_1}},
  { muxButtons.pin(5),   {22, CHANNEL_1}},
  { muxButtons.pin(6),   {23, CHANNEL_1}},
  { muxButtons.pin(7),   {24, CHANNEL_1}},
  { muxButtons.pin(8),   {25, CHANNEL_1}},
  { muxButtons.pin(9),   {26, CHANNEL_1}},
  { muxButtons.pin(10),  {27, CHANNEL_1}},
  { muxButtons.pin(11),  {28, CHANNEL_1}},
  { muxButtons.pin(12),  {29, CHANNEL_1}},
  { muxButtons.pin(13),  {30, CHANNEL_1}},
  { muxButtons.pin(14),  {31, CHANNEL_1}},
  { muxButtons.pin(15),  {32, CHANNEL_1}}
};

CCPotentiometer potentiometers0_15[] = {
  { muxPots.pin(0),   {1, CHANNEL_1}},
  { muxPots.pin(1),   {2, CHANNEL_1}},
  { muxPots.pin(2),   {3, CHANNEL_1}},
  { muxPots.pin(3),   {4, CHANNEL_1}},
  { muxPots.pin(4),   {5, CHANNEL_1}},
  { muxPots.pin(5),   {6, CHANNEL_1}},
  { muxPots.pin(6),   {7, CHANNEL_1}},
  { muxPots.pin(7),   {8, CHANNEL_1}},
  { muxPots.pin(8),   {9, CHANNEL_1}},
  { muxPots.pin(9),   {10, CHANNEL_1}},
  { muxPots.pin(10),  {11, CHANNEL_1}},
  { muxPots.pin(11),  {12, CHANNEL_1}},
  { muxPots.pin(12),  {13, CHANNEL_1}},
  { muxPots.pin(13),  {14, CHANNEL_1}},
  { muxPots.pin(14),  {15, CHANNEL_1}},
  { muxPots.pin(15),  {16, CHANNEL_1}}
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();

  for (uint16_t i = 0; i < ledCount; i++) {
    if (buttons0_15[i].getState()) {
      colors[i] = rgb_color(255, 255, 0);
    }
    else {
      colors[i] = rgb_color(0, 0, 0);
    }
  }

  ledStrip.write(colors, ledCount, brightness);

}
