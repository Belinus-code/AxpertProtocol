// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// PBDV<nn.n> (3.13): battery voltage at which the device switches back to
// battery mode. 00.0 means "battery is full" (charging in float mode).
struct SetBatteryVoltageBackToBatteryRequest {
    float voltage;

    size_t build(char* buf, size_t bufCapacity) const;
};
