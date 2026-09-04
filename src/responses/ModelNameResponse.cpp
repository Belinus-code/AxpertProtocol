// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "ModelNameResponse.h"

#include <string.h>

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool ModelNameResponse::parse(const uint8_t* raw, size_t len, ModelNameResponse& out) {
    const char* text = reinterpret_cast<const char*>(raw);

    if (len == 4 && strncmp(text, "(NAK", 4) == 0) return false;
    if (len < 3 || text[0] != '(') return false;

    // "(MMMMM-NNNN": find the '-' separating the name from the rated VA.
    size_t dash = 0;
    for (size_t i = 1; i < len; ++i) {
        if (text[i] == '-') {
            dash = i;
            break;
        }
    }
    if (dash == 0 || dash + 1 >= len) return false;

    if (!copyToken(text + 1, dash - 1, out.name, sizeof(out.name))) return false;
    out.ratedOutputVa = static_cast<uint16_t>(parseUIntToken(text + dash + 1, len - dash - 1));
    return true;
}

bool GeneralModelNameResponse::parse(const uint8_t* raw, size_t len, GeneralModelNameResponse& out) {
    const char* text = reinterpret_cast<const char*>(raw);

    if (len == 4 && strncmp(text, "(NAK", 4) == 0) return false;
    if (len != 4 || text[0] != '(') return false; // '(' + 3 decimal digits

    out.modelCode = static_cast<uint16_t>(parseUIntToken(text + 1, 3));
    return true;
}
