// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// PBEQP<nnn> (3.31): equalization period, in days (0-90 per the protocol -
// not validated here).
struct SetBatteryEqualizationPeriodRequest {
    uint16_t days;

    size_t build(char* buf, size_t bufCapacity) const;
};
