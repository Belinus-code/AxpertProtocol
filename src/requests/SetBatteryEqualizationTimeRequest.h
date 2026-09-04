// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// PBEQT<nnn> (3.30): equalization time, in minutes (5-900, in 5-minute
// steps per the protocol - not validated here).
struct SetBatteryEqualizationTimeRequest {
    uint16_t minutes;

    size_t build(char* buf, size_t bufCapacity) const;
};
