// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// Shared by QET/QEY/QEM/QED (2.25-2.28, PV generated energy) and
// QLT/QLY/QLM/QLD (2.29-2.32, output load energy) - all eight commands
// reply with a single 8-digit Wh value; only the query itself (and its
// optional year/month/day parameter) differs.
struct EnergyResponse {
    uint32_t energyWh;

    static bool parse(const uint8_t* raw, size_t len, EnergyResponse& out);
};
