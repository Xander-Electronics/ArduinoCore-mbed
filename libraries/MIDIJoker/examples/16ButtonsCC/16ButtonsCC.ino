#include <Control_Surface.h>

USBMIDI_Interface midi;             //Init MIDI over USB

CD74HC4067 mux0_15 = {
  A1,                               //Digital pin
  {MUX2_A, MUX2_B, MUX2_C, MUX2_D}  //Address pins
};
/*
 * - When the button on pin is pressed, a MIDI Control Change message with a 
 *   value of 0x7F (127) is sent
 * - When the button on pin is pressed again, a MIDI Control Change message with a 
 *   value of 0x00 (0) is sent
 */

CCButtonLatched buttons0_15[] = {
  { mux0_15.pin(0),   {17, CHANNEL_1}},
  { mux0_15.pin(1),   {18, CHANNEL_1}},
  { mux0_15.pin(2),   {19, CHANNEL_1}},
  { mux0_15.pin(3),   {20, CHANNEL_1}},
  { mux0_15.pin(4),   {21, CHANNEL_1}},
  { mux0_15.pin(5),   {22, CHANNEL_1}},
  { mux0_15.pin(6),   {23, CHANNEL_1}},
  { mux0_15.pin(7),   {24, CHANNEL_1}},
  { mux0_15.pin(8),   {25, CHANNEL_1}},
  { mux0_15.pin(9),   {26, CHANNEL_1}},
  { mux0_15.pin(10),  {27, CHANNEL_1}},
  { mux0_15.pin(11),  {28, CHANNEL_1}},
  { mux0_15.pin(12),  {29, CHANNEL_1}},
  { mux0_15.pin(13),  {30, CHANNEL_1}},
  { mux0_15.pin(14),  {31, CHANNEL_1}},
  { mux0_15.pin(15),  {32, CHANNEL_1}}
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
