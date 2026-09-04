// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// POPM<nn> (3.10): setting output mode.
struct SetOutputModeRequest {
    AxpertOutputMode mode;

    size_t build(char* buf, size_t bufCapacity) const;
};
