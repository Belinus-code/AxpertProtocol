// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// MUCHGC<mnnn> (3.6): setting max utility (AC) charging current. Note the
// protocol keeps the "(only for MKSII)" qualifier on this exact 4-digit
// form even after the VMIII/KING-specific variants were removed.
// `unitIndex` is the parallel unit number (0 for a single system);
// `currentAmps` is the selectable value from QMUCHGCR.
struct SetMaxUtilityChargingCurrentRequest {
    uint8_t unitIndex;
    uint16_t currentAmps;

    size_t build(char* buf, size_t bufCapacity) const;
};
