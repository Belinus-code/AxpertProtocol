// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PCVV<nn.n> (3.19): setting battery C.V. (constant voltage) charging
// voltage.
struct SetBatteryCvChargingVoltageRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
