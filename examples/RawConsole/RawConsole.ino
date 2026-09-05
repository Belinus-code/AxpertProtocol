// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include <AxpertProtocol.h>

// Any HardwareSerial works out of the box. Here we use Serial1 (e.g. Mega,
// Due, ESP32) so `Serial` stays free for the console below.
AxpertDevice inverter(Serial1);

void setup() {
    Serial.begin(115200);
    inverter.begin(); // configures Serial1 at the protocol's 2400 8N1 for you
    Serial.println(F("AxpertProtocol raw console"));
    Serial.println(F("Type a bare command (e.g. QPIGS, QPIRI, PSDV048.0) and press enter."));
}

void loop() {
    static char line[64];
    static size_t len = 0;

    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\r') continue;
        if (c == '\n') {
            line[len] = '\0';
            if (len > 0) {
                char response[128];
                // sendRawCommand() is the same escape hatch every typed
                // query*()/set*() method is itself built on - see the
                // "Sending raw commands" section in the README for details
                // (including the appendCRC parameter, left at its default
                // `true` here).
                if (inverter.sendRawCommand(line, response, sizeof(response))) {
                    Serial.print(F("-> "));
                    Serial.println(response);
                } else {
                    Serial.println(F("-> (no reply - timeout, bad CRC, or too small a response buffer)"));
                }
            }
            len = 0;
        } else if (len + 1 < sizeof(line)) {
            line[len++] = c;
        }
    }
}
