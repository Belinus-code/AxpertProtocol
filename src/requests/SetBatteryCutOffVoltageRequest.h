// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PSDV<nn.n> (3.18): setting battery cut-off voltage (battery under voltage).
struct SetBatteryCutOffVoltageRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
