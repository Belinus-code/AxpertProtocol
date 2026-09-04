// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "FirmwareVersionResponse.h"

#include <string.h>

namespace {

// Hex parsing is only needed here (every other response field is decimal),
// so this stays local instead of joining AxpertParsing.
uint8_t hexDigitValue(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
    return 0xFF;
}

bool parseHex(const char* text, size_t len, uint32_t& out) {
    uint32_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t digit = hexDigitValue(text[i]);
        if (digit == 0xFF) return false;
        value = (value << 4) | digit;
    }
    out = value;
    return true;
}

} // namespace

bool FirmwareVersionResponse::parse(const uint8_t* raw, size_t len, FirmwareVersionResponse& out) {
    // "(VERFW:NNNNN.NN": '(' + "VERFW:" + 5 hex digits + '.' + 2 hex digits.
    static const char prefix[] = "(VERFW:";
    const size_t prefixLen = sizeof(prefix) - 1; // 7, excludes the null terminator
    if (len != prefixLen + 5 + 1 + 2) return false;

    const char* text = reinterpret_cast<const char*>(raw);
    if (strncmp(text, prefix, prefixLen) != 0) return false;
    if (text[prefixLen + 5] != '.') return false;

    uint32_t series;
    uint32_t version;
    if (!parseHex(text + prefixLen, 5, series)) return false;
    if (!parseHex(text + prefixLen + 6, 2, version)) return false;

    out.seriesNumber = series;
    out.version = static_cast<uint8_t>(version);
    return true;
}
