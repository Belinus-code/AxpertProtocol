// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "FlagStatusResponse.h"

bool FlagStatusResponse::parse(const uint8_t* raw, size_t len, FlagStatusResponse& out) {
    // "(ExxxDxxx": letters between 'E' and 'D' are enabled, letters after
    // 'D' are disabled. Every known letter always appears in one group or
    // the other, but unknown/reserved letters are ignored for forward
    // compatibility.
    if (len < 2 || raw[0] != '(' || raw[1] != 'E') return false;
    out = FlagStatusResponse{}; // default every flag to false first

    const char* text = reinterpret_cast<const char*>(raw);
    bool enabled = true;
    for (size_t i = 2; i < len; ++i) {
        char c = text[i];
        if (c == 'D') {
            enabled = false;
            continue;
        }
        switch (c) {
            case 'a': out.silenceBuzzerEnabled = enabled; break;
            case 'b': out.overloadBypassEnabled = enabled; break;
            case 'k': out.lcdEscapeToDefaultEnabled = enabled; break;
            case 'u': out.overloadRestartEnabled = enabled; break;
            case 'v': out.overTemperatureRestartEnabled = enabled; break;
            case 'x': out.backlightOnEnabled = enabled; break;
            case 'y': out.alarmOnPrimarySourceInterruptEnabled = enabled; break;
            case 'z': out.faultCodeRecordEnabled = enabled; break;
            default: break; // unknown/reserved letter
        }
    }
    return true;
}
