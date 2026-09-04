// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QBEQI (2.22): battery equalization status parameters. Two fields in the
// wire format (F/FFF and H/HHH) are documented only as "reserved" - they
// are consumed while parsing but intentionally not exposed here.
struct BatteryEqualizationStatusResponse {
    bool enabled;                          // B
    uint16_t equalizationTimeMinutes;      // CCC
    uint16_t equalizationPeriodDays;       // DDD
    uint16_t equalizationMaxCurrent;       // EEE, in A
    float equalizationVoltage;             // GG.GG, in V
    uint16_t equalizationOverTimeMinutes;  // III
    bool active;                           // J
    uint32_t equalizationElapseTimeHours;  // KKKK

    static bool parse(const uint8_t* raw, size_t len, BatteryEqualizationStatusResponse& out);
};
