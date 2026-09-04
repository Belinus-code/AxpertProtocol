// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PSPB<n> (3.25): setting solar power balance.
struct SetPvPowerBalanceRequest {
    AxpertPvPowerBalance balance;

    size_t build(char* buf, size_t bufCapacity) const;
};
