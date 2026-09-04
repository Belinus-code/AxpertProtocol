// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QID (2.2): "(XXXXXXXXXXXXXX" - 14-char serial number.
// QSID (2.3): "(NNXXXXXXXXXXXXXXXXXXXX" - 2-digit valid length + 20-char
// (zero-padded) serial number. AXPERT_SERIAL_MAX_LEN covers both.
#define AXPERT_SERIAL_MAX_LEN 20

struct SerialNumberResponse {
    char serial[AXPERT_SERIAL_MAX_LEN + 1]; // null-terminated
    uint8_t length; // valid length; for QID this is always 14

    static bool parse(const uint8_t* raw, size_t len, SerialNumberResponse& out);         // QID
    static bool parseExtended(const uint8_t* raw, size_t len, SerialNumberResponse& out); // QSID
};
