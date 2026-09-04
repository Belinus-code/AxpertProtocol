// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBatteryEqualizationOverTimeRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBatteryEqualizationOverTimeRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PBEQOT")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, minutes, 3)) return 0;
    return offset;
}
