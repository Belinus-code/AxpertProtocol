// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QFLAG (2.9): "(ExxxDxxx" - each flag is reported as enabled (grouped
// after 'E') or disabled (grouped after 'D'). Represented here as
// individual bools instead of the raw letter-grouped string.
struct FlagStatusResponse {
    bool silenceBuzzerEnabled;                 // a
    bool overloadBypassEnabled;                // b
    bool lcdEscapeToDefaultEnabled;            // k
    bool overloadRestartEnabled;               // u
    bool overTemperatureRestartEnabled;        // v
    bool backlightOnEnabled;                   // x
    bool alarmOnPrimarySourceInterruptEnabled; // y
    bool faultCodeRecordEnabled;               // z

    static bool parse(const uint8_t* raw, size_t len, FlagStatusResponse& out);
};
