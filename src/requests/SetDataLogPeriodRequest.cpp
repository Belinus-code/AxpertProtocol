// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetDataLogPeriodRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetDataLogPeriodRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PDL")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, minutes, 2)) return 0;
    return offset;
}
