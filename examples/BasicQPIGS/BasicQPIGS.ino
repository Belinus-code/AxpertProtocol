// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include <AxpertProtocol.h>

// Any HardwareSerial works out of the box. Here we use Serial1 (e.g. Mega,
// Due, ESP32) so `Serial` stays free for debug output via the USB monitor.
AxpertDevice inverter(Serial1);

void setup() {
    Serial.begin(115200);
    inverter.begin(); // configures Serial1 at the protocol's 2400 8N1 for you
}

void loop() {
    GeneralStatusResponse status;
    if (inverter.queryGeneralStatus(status)) {
        Serial.print("Battery voltage: ");
        Serial.println(status.batteryVoltage);
    } else {
        Serial.println("QPIGS query failed (timeout or bad CRC)");
    }
    delay(1000);
}
