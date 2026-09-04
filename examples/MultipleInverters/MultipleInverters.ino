// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include <AxpertProtocol.h>

// Each AxpertDevice instance is independent - no shared/static state - so
// using several trackers at once is just a matter of giving each its own
// Stream (two hardware UARTs here; could just as well be two SoftwareSerial
// instances).
AxpertDevice inverterA(Serial1);
AxpertDevice inverterB(Serial2);

void setup() {
    Serial.begin(115200);
    inverterA.begin(); // configures Serial1 at 2400 8N1
    inverterB.begin(); // configures Serial2 at 2400 8N1

    // Changes this device's default timeout for every call that doesn't
    // pass its own override below.
    inverterA.setDefaultTimeout(500);
}

void loop() {
    GeneralStatusResponse statusA, statusB;
    bool okA = inverterA.queryGeneralStatus(statusA);
    // Per-call override: this one query gets more time than inverterB's
    // own default, without changing that default for its other calls.
    bool okB = inverterB.queryGeneralStatus(statusB, /*timeoutMs=*/1000);

    if (okA) Serial.println(statusA.batteryVoltage);
    if (okB) Serial.println(statusB.batteryVoltage);

    delay(1000);
}
