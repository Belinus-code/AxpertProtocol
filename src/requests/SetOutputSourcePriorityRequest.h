// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// POP<NN> (3.9): setting device output source priority.
struct SetOutputSourcePriorityRequest {
    AxpertOutputSourcePriority priority;

    size_t build(char* buf, size_t bufCapacity) const;
};
