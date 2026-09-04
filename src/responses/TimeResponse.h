// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QT (2.19): "(YYYYMMDDHHMMSS" - current device date/time. Unlike almost
// every other command, QT's request and reply carry no CRC at all (see
// protocol 2.19) - keep that in mind when wiring up the transaction.
struct TimeResponse {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    static bool parse(const uint8_t* raw, size_t len, TimeResponse& out);
};
