// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PE<xxx> / PD<xxx> (3.3): enables (PE) or disables (PD) any subset of the
// listed flags in one command. Every field set to `true` is included as
// its letter in the outgoing string; all included flags move in the same
// direction, chosen by `enable`. Uses the same flag set as
// FlagStatusResponse (c/d/m were removed from this protocol revision).
struct SetFlagsRequest {
    bool enable; // true -> "PE...", false -> "PD..."

    bool silenceBuzzer;                 // a
    bool overloadBypass;                // b
    bool lcdEscapeToDefault;            // k
    bool overloadRestart;               // u
    bool overTemperatureRestart;        // v
    bool backlightOn;                   // x
    bool alarmOnPrimarySourceInterrupt; // y
    bool faultCodeRecord;               // z

    // Writes the command payload (e.g. "PEbuxz") into `buf`. Returns the
    // number of bytes written, or 0 if it didn't fit `bufCapacity` or no
    // flag was selected.
    size_t build(char* buf, size_t bufCapacity) const;
};
