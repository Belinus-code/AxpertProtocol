// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// POPV<nnnn> (3.8): setting device output rating voltage. `voltageTimesTen`
// is the voltage already scaled by 10 (2200/2300/2400 for 220/230/240V).
struct SetOutputRatingVoltageRequest {
    uint16_t voltageTimesTen;

    size_t build(char* buf, size_t bufCapacity) const;
};
