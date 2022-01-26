#include <Control_Surface.h>

USBMIDI_Interface midi;             //Init MIDI over USB

CD74HC4067 mux0_15 = {
  A0,                               //Analog pin
  {MUX1_A, MUX1_B, MUX1_C, MUX1_D}  //Address pins
};

CCPotentiometer potentiometers0_15[] = {
  { mux0_15.pin(0),   {1, CHANNEL_1}},
  { mux0_15.pin(1),   {2, CHANNEL_1}},
  { mux0_15.pin(2),   {3, CHANNEL_1}},
  { mux0_15.pin(3),   {4, CHANNEL_1}},
  { mux0_15.pin(4),   {5, CHANNEL_1}},
  { mux0_15.pin(5),   {6, CHANNEL_1}},
  { mux0_15.pin(6),   {7, CHANNEL_1}},
  { mux0_15.pin(7),   {8, CHANNEL_1}},
  { mux0_15.pin(8),   {9, CHANNEL_1}},
  { mux0_15.pin(9),   {10, CHANNEL_1}},
  { mux0_15.pin(10),  {11, CHANNEL_1}},
  { mux0_15.pin(11),  {12, CHANNEL_1}},
  { mux0_15.pin(12),  {13, CHANNEL_1}},
  { mux0_15.pin(13),  {14, CHANNEL_1}},
  { mux0_15.pin(14),  {15, CHANNEL_1}},
  { mux0_15.pin(15),  {16, CHANNEL_1}}
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
