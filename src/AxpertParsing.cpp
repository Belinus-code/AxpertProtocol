// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AxpertParsing.h"

#include <stdlib.h>
#include <string.h>

namespace AxpertParsing {

bool nextToken(const char*& cursor, const char* end, const char*& tokenStart, size_t& tokenLen) {
    while (cursor < end && *cursor == ' ') ++cursor;
    if (cursor >= end) return false;
    tokenStart = cursor;
    while (cursor < end && *cursor != ' ') ++cursor;
    tokenLen = static_cast<size_t>(cursor - tokenStart);
    return tokenLen > 0;
}

float parseFloatToken(const char* token, size_t len) {
    char buf[16];
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;
    memcpy(buf, token, len);
    buf[len] = '\0';
    return static_cast<float>(atof(buf));
}

uint32_t parseUIntToken(const char* token, size_t len) {
    uint32_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        char c = token[i];
        if (c < '0' || c > '9') break;
        value = value * 10 + static_cast<uint32_t>(c - '0');
    }
    return value;
}

bool copyToken(const char* token, size_t len, char* dest, size_t destCapacity) {
    if (destCapacity == 0) return false;
    bool fits = len < destCapacity;
    size_t copyLen = fits ? len : destCapacity - 1;
    memcpy(dest, token, copyLen);
    dest[copyLen] = '\0';
    return fits;
}

} // namespace AxpertParsing
