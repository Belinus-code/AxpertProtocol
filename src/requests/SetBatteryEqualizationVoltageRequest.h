// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBEQV<nn.nn> (3.32): equalization voltage.
struct SetBatteryEqualizationVoltageRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
