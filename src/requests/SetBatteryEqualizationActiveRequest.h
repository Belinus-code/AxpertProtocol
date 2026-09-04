// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBEQA<n> (3.34): activate or deactivate battery equalization right now.
struct SetBatteryEqualizationActiveRequest {
    bool active;

    size_t build(char* buf, size_t bufCapacity) const;
};
