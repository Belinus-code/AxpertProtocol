// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBCV<nn.n> (3.12): battery voltage at which the device switches back to
// utility.
struct SetBatteryVoltageBackToUtilityRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
