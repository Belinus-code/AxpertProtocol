// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// F<nn> (3.7): setting inverter output rating frequency, 50 or 60 (Hz).
struct SetOutputFrequencyRequest {
    uint8_t frequencyHz;

    size_t build(char* buf, size_t bufCapacity) const;
};
