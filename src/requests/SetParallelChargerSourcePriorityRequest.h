// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../AxpertTypes.h"

// PPCP<MNN> (3.15): setting device charger priority for one unit in a
// parallel system. `unitIndex` is the parallel unit number.
struct SetParallelChargerSourcePriorityRequest {
    uint8_t unitIndex;
    AxpertChargerSourcePriority priority;

    size_t build(char* buf, size_t bufCapacity) const;
};
