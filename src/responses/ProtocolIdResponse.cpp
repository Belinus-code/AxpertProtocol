// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "ProtocolIdResponse.h"

#include "../AxpertParsing.h"

bool ProtocolIdResponse::parse(const uint8_t* raw, size_t len, ProtocolIdResponse& out) {
    // "(PI<NN>": '(' + "PI" + 2 decimal digits.
    if (len != 5) return false;
    const char* text = reinterpret_cast<const char*>(raw);
    if (text[0] != '(' || text[1] != 'P' || text[2] != 'I') return false;
    if (text[3] < '0' || text[3] > '9' || text[4] < '0' || text[4] > '9') return false;

    out.protocolId = static_cast<uint8_t>(AxpertParsing::parseUIntToken(text + 3, 2));
    return true;
}
