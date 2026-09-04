// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBEQE<n> (3.29): enable or disable battery equalization.
struct SetBatteryEqualizationEnabledRequest {
    bool enabled;

    size_t build(char* buf, size_t bufCapacity) const;
};
