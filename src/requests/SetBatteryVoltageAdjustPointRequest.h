// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// BTA1<nnn.nn> / BTA2<nnn.nn> (3.21/3.22): battery voltage adjust point one
// and two. Identical shape for both commands, so one struct covers them;
// `point` (1 or 2) picks which command letter gets emitted.
struct SetBatteryVoltageAdjustPointRequest {
    uint8_t point; // 1 or 2
    float voltage;

    // Returns 0 (without writing anything) if `point` isn't 1 or 2.
    size_t build(char* buf, size_t bufCapacity) const;
};
