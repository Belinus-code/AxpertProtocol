// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// PCVT<nnn> (3.35): set max charging time at C.V. stage, in minutes.
struct SetMaxChargingTimeAtCvRequest {
    uint16_t minutes;

    size_t build(char* buf, size_t bufCapacity) const;
};
