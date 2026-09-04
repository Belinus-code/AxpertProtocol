// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// PCP<NN> (3.14): setting device charger priority (the KING-specific
// SBL/UCB scheme this command used to also support was removed from this
// protocol revision - AxpertChargerSourcePriority is the only option now).
struct SetChargerSourcePriorityRequest {
    AxpertChargerSourcePriority priority;

    size_t build(char* buf, size_t bufCapacity) const;
};
