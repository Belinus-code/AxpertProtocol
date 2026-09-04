// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PPVOKC<n> (3.24): setting PV OK condition for parallel systems.
struct SetPvOkConditionRequest {
    AxpertPvOkConditionForParallel condition;

    size_t build(char* buf, size_t bufCapacity) const;
};
