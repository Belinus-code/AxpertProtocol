// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "ChargingCurrentOptionsResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool ChargingCurrentOptionsResponse::parse(const uint8_t* raw, size_t len, ChargingCurrentOptionsResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    out.count = 0;
    while (out.count < AXPERT_MAX_CHARGING_CURRENT_OPTIONS && nextToken(cursor, end, tok, tokLen)) {
        out.values[out.count] = static_cast<uint16_t>(parseUIntToken(tok, tokLen));
        ++out.count;
    }
    return out.count > 0;
}
