// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "TimeResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool TimeResponse::parse(const uint8_t* raw, size_t len, TimeResponse& out) {
    // "(YYYYMMDDHHMMSS" - fixed-width, no CRC.
    if (len != 15 || raw[0] != '(') return false;
    const char* text = reinterpret_cast<const char*>(raw) + 1;

    out.year = static_cast<uint16_t>(parseUIntToken(text, 4));
    out.month = static_cast<uint8_t>(parseUIntToken(text + 4, 2));
    out.day = static_cast<uint8_t>(parseUIntToken(text + 6, 2));
    out.hour = static_cast<uint8_t>(parseUIntToken(text + 8, 2));
    out.minute = static_cast<uint8_t>(parseUIntToken(text + 10, 2));
    out.second = static_cast<uint8_t>(parseUIntToken(text + 12, 2));
    return true;
}
