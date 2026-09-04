// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SerialNumberResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool SerialNumberResponse::parse(const uint8_t* raw, size_t len, SerialNumberResponse& out) {
    // "(XXXXXXXXXXXXXX": '(' + 14 serial characters.
    if (len != 15 || raw[0] != '(') return false;
    const char* text = reinterpret_cast<const char*>(raw);

    if (!copyToken(text + 1, 14, out.serial, sizeof(out.serial))) return false;
    out.length = 14;
    return true;
}

bool SerialNumberResponse::parseExtended(const uint8_t* raw, size_t len, SerialNumberResponse& out) {
    // "(NNXXXXXXXXXXXXXXXXXXXX": '(' + 2-digit valid length + 20 chars.
    if (len != 23 || raw[0] != '(') return false;
    const char* text = reinterpret_cast<const char*>(raw);

    out.length = static_cast<uint8_t>(parseUIntToken(text + 1, 2));
    if (!copyToken(text + 3, 20, out.serial, sizeof(out.serial))) return false;
    return true;
}
