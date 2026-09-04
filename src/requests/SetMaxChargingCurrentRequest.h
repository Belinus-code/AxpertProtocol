// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// MNCHGC<mnnn> (3.5): setting max (combined) charging current. `unitIndex`
// is the parallel unit number (0 for a single, non-parallel system);
// `currentAmps` is the selectable value from QMCHGCR.
struct SetMaxChargingCurrentRequest {
    uint8_t unitIndex;
    uint16_t currentAmps;

    size_t build(char* buf, size_t bufCapacity) const;
};
