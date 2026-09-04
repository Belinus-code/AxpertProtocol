// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include <AxpertProtocol.h>

// ESP32's hardware UARTs can be remapped to almost any GPIO pair, so
// "arbitrary RX/TX pins" doesn't need software bit-banging there like it
// does on AVR (ESP32 has no built-in SoftwareSerial anyway). This
// constructor is only available when compiling for ESP32.
AxpertDevice inverter(Serial2, /*rxPin=*/16, /*txPin=*/17);

void setup() {
    Serial.begin(115200);
    inverter.begin(); // calls Serial2.begin(2400, SERIAL_8N1, 16, 17) for you
}

void loop() {
    GeneralStatusResponse status;
    if (inverter.queryGeneralStatus(status)) {
        Serial.println(status.batteryVoltage);
    }
    delay(1000);
}
