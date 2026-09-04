// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PE<xxx> / PD<xxx> (3.3): enable/disable device flags. Symmetric with
// FlagStatusResponse (2.9), which always reports all 8 flags at once -
// every field here addresses one of the same 8 flags, and can
// independently be set to Enabled, Disabled, or left at the default
// Unchanged. No need to query the current state first just to change one
// flag: leave the other 7 at Unchanged.
//
// The protocol itself only has two commands, PE<xxx> (enable a list of
// letters) and PD<xxx> (disable a list of letters) - there's no single
// wire command that sets some flags on and others off at once. So unlike
// every other Request struct, this one doesn't have a single build(): it
// has buildEnableCommand() and buildDisableCommand(), and
// AxpertDevice::setFlags() sends whichever of the two actually have
// something to say (0, 1, or both).
struct SetFlagsRequest {
    AxpertFlagState silenceBuzzer;                 // a
    AxpertFlagState overloadBypass;                // b
    AxpertFlagState lcdEscapeToDefault;             // k
    AxpertFlagState overloadRestart;                // u
    AxpertFlagState overTemperatureRestart;         // v
    AxpertFlagState backlightOn;                    // x
    AxpertFlagState alarmOnPrimarySourceInterrupt;  // y
    AxpertFlagState faultCodeRecord;                // z

    // Writes "PE<xxx>" (only the flags set to Enabled) into `buf`. Returns
    // the number of bytes written, or 0 if it didn't fit `bufCapacity` or
    // no flag is Enabled.
    size_t buildEnableCommand(char* buf, size_t bufCapacity) const;

    // Writes "PD<xxx>" (only the flags set to Disabled) into `buf`.
    // Returns the number of bytes written, or 0 if it didn't fit
    // `bufCapacity` or no flag is Disabled.
    size_t buildDisableCommand(char* buf, size_t bufCapacity) const;
};
