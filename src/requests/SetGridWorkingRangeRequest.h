// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PGR<NN> (3.16): setting device grid working range.
struct SetGridWorkingRangeRequest {
    AxpertInputVoltageRange range;

    size_t build(char* buf, size_t bufCapacity) const;
};
