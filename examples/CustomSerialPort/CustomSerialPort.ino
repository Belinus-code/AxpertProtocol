// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include <AxpertProtocol.h>

// For boards with only one hardware UART (e.g. Uno): use any two free pins.
// This constructor owns an internal SoftwareSerial on the given pins and
// configures it for you via begin() below - AVR only, since that's the
// only architecture with a built-in SoftwareSerial (see ESP32PinRemap
// example for the ESP32 equivalent, which uses a real hardware UART
// instead).
AxpertDevice inverter(10, 11); // RX, TX - pick any free pins

// If you need the SoftwareSerial object yourself too (e.g. to also print
// raw bytes for debugging), construct it and pass it in instead:
//   SoftwareSerial inverterSerial(10, 11);
//   AxpertDevice inverter(inverterSerial);
// begin() configures that one for you as well.

void setup() {
    Serial.begin(115200);
    inverter.begin(); // configures the internal SoftwareSerial at 2400 8N1
}

void loop() {
    DeviceModeResponse mode;
    if (inverter.queryDeviceMode(mode)) {
        // TODO: switch on mode.mode (AxpertDeviceMode) and act on it.
    }
    delay(1000);
}
