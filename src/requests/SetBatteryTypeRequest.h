// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PBT<NN> (3.17): setting battery type.
struct SetBatteryTypeRequest {
    AxpertBatteryType type;

    size_t build(char* buf, size_t bufCapacity) const;
};
