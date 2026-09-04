// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBFT<nn.n> (3.20): setting battery float charging voltage.
struct SetBatteryFloatChargingVoltageRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
