#include <Control_Surface.h>

USBMIDI_Interface midi;             //Init MIDI over USB

CD74HC4067 mux0_15 = {
  A0,                               //Analog pin
  {MUX1_A, MUX1_B, MUX1_C, MUX1_D}  //Address pins
};

CD74HC4067 mux16_31 = {
  A0,                               //Analog pin
  {MUX2_A, MUX2_B, MUX2_C, MUX2_D}  //Address pins
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

CCPotentiometer potentiometers16_31[] = {
  { mux16_31.pin(0),   {17, CHANNEL_1}},
  { mux16_31.pin(1),   {18, CHANNEL_1}},
  { mux16_31.pin(2),   {19, CHANNEL_1}},
  { mux16_31.pin(3),   {20, CHANNEL_1}},
  { mux16_31.pin(4),   {21, CHANNEL_1}},
  { mux16_31.pin(5),   {22, CHANNEL_1}},
  { mux16_31.pin(6),   {23, CHANNEL_1}},
  { mux16_31.pin(7),   {24, CHANNEL_1}},
  { mux16_31.pin(8),   {25, CHANNEL_1}},
  { mux16_31.pin(9),   {26, CHANNEL_1}},
  { mux16_31.pin(10),  {27, CHANNEL_1}},
  { mux16_31.pin(11),  {28, CHANNEL_1}},
  { mux16_31.pin(12),  {29, CHANNEL_1}},
  { mux16_31.pin(13),  {30, CHANNEL_1}},
  { mux16_31.pin(14),  {31, CHANNEL_1}},
  { mux16_31.pin(15),  {32, CHANNEL_1}}
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
