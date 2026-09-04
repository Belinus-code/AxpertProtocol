// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBatteryEqualizationActiveRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBatteryEqualizationActiveRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PBEQA")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, active ? 1 : 0, 1)) return 0;
    return offset;
}
