// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// PDL<nn> (3.28): data log stored period, in minutes. Allowed values per
// the protocol: 3, 5, 10, 20, 30, 60 - not validated here, the device will
// NAK an out-of-range value.
struct SetDataLogPeriodRequest {
    uint8_t minutes;

    size_t build(char* buf, size_t bufCapacity) const;
};
