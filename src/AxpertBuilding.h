// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// Shared helpers for building the request payloads sent to the device
// (before AxpertCRC::appendFramed() adds the CRC + trailing <cr>). Every
// Request::build() writes into a caller-supplied buffer via these, rather
// than each reimplementing decimal/fixed-point formatting.
namespace AxpertBuilding {

// Appends `text` at buf[*offset], advancing *offset and null-terminating.
// Returns false (leaving buf/*offset unchanged) if it wouldn't fit.
bool appendText(char* buf, size_t bufCapacity, size_t* offset, const char* text);

// Appends `value` as exactly `width` zero-padded decimal digits (e.g.
// width=3, value=7 -> "007"). Returns false if it wouldn't fit in `buf`,
// or if `value` needs more than `width` digits to represent.
bool appendUInt(char* buf, size_t bufCapacity, size_t* offset, uint32_t value, uint8_t width);

// Appends `value` with exactly `decimals` digits after the decimal point
// (e.g. decimals=2, value=24.5 -> "24.50"), rounding to that precision.
// The integer part is written with as many digits as it needs (no
// padding). Doesn't use printf's float support, which needs extra linker
// flags on AVR.
bool appendFixed(char* buf, size_t bufCapacity, size_t* offset, float value, uint8_t decimals);

} // namespace AxpertBuilding
