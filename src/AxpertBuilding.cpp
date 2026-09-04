// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AxpertBuilding.h"

#include <string.h>

namespace AxpertBuilding {

bool appendText(char* buf, size_t bufCapacity, size_t* offset, const char* text) {
    size_t len = strlen(text);
    if (*offset + len >= bufCapacity) return false; // leave room for the null terminator
    memcpy(buf + *offset, text, len);
    *offset += len;
    buf[*offset] = '\0';
    return true;
}

bool appendUInt(char* buf, size_t bufCapacity, size_t* offset, uint32_t value, uint8_t width) {
    if (width == 0 || width > 9) return false;
    if (*offset + width >= bufCapacity) return false;

    char digits[9];
    for (uint8_t i = 0; i < width; ++i) {
        digits[width - 1 - i] = static_cast<char>('0' + (value % 10));
        value /= 10;
    }
    if (value != 0) return false; // didn't fit in `width` digits

    memcpy(buf + *offset, digits, width);
    *offset += width;
    buf[*offset] = '\0';
    return true;
}

bool appendFixed(char* buf, size_t bufCapacity, size_t* offset, float value, uint8_t decimals) {
    bool negative = value < 0.0f;
    if (negative) value = -value;

    uint32_t scale = 1;
    for (uint8_t i = 0; i < decimals; ++i) scale *= 10;

    uint32_t scaled = static_cast<uint32_t>(value * static_cast<float>(scale) + 0.5f);
    uint32_t intPart = scaled / scale;
    uint32_t fracPart = scaled % scale;

    char intDigits[10];
    size_t intLen = 0;
    if (intPart == 0) {
        intDigits[intLen++] = '0';
    } else {
        char rev[10];
        size_t revLen = 0;
        while (intPart > 0 && revLen < sizeof(rev)) {
            rev[revLen++] = static_cast<char>('0' + (intPart % 10));
            intPart /= 10;
        }
        while (revLen > 0) intDigits[intLen++] = rev[--revLen];
    }

    size_t needed = (negative ? 1u : 0u) + intLen + (decimals > 0 ? 1u + decimals : 0u);
    if (*offset + needed >= bufCapacity) return false;

    if (negative) buf[(*offset)++] = '-';
    memcpy(buf + *offset, intDigits, intLen);
    *offset += intLen;

    if (decimals > 0) {
        buf[(*offset)++] = '.';
        char fracDigits[8];
        for (uint8_t i = 0; i < decimals; ++i) {
            fracDigits[decimals - 1 - i] = static_cast<char>('0' + (fracPart % 10));
            fracPart /= 10;
        }
        memcpy(buf + *offset, fracDigits, decimals);
        *offset += decimals;
    }
    buf[*offset] = '\0';
    return true;
}

} // namespace AxpertBuilding
