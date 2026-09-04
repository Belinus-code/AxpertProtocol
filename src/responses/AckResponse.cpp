// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AckResponse.h"
#include "../AxpertParsing.h"
#include <string.h>

using namespace AxpertParsing;

bool AckResponse::parse(const uint8_t* raw, size_t len, AckResponse& out) {
    if (len == 0 || raw[0] != '(') return false;
    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    if (tokLen == 3 && strncmp(tok, "ACK", 3) == 0) {
        out.ok = true;
    } else if (tokLen == 3 && strncmp(tok, "NAK", 3) == 0) {
        out.ok = false;
    } else {
        return false; // Invalid token
    }
    return true;
}
