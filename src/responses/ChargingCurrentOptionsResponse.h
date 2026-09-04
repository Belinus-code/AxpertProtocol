// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// Generous upper bound - the protocol allows an arbitrary number of values
// ("More value can be added, make sure there is a space character between
// every value.") but real devices report a handful.
#define AXPERT_MAX_CHARGING_CURRENT_OPTIONS 16

// QMCHGCR (2.14) / QMUCHGCR (2.15): "(AAA BBB CCC DDD......" - a
// variable-length, space-separated list of selectable current values (A).
// Both commands share this exact response shape, so one struct covers
// both.
struct ChargingCurrentOptionsResponse {
    uint16_t values[AXPERT_MAX_CHARGING_CURRENT_OPTIONS];
    uint8_t count;

    static bool parse(const uint8_t* raw, size_t len, ChargingCurrentOptionsResponse& out);
};
