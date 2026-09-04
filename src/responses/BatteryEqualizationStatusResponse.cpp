// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "BatteryEqualizationStatusResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool BatteryEqualizationStatusResponse::parse(const uint8_t* raw, size_t len, BatteryEqualizationStatusResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.enabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationTimeMinutes = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationPeriodDays = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationMaxCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false; // FFF, reserved

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false; // HHH, reserved

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationOverTimeMinutes = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.active = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.equalizationElapseTimeHours = parseUIntToken(tok, tokLen);

    return true;
}
