// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetMaxChargingCurrentRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetMaxChargingCurrentRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "MNCHGC")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, unitIndex, 1)) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, currentAmps, 3)) return 0;
    return offset;
}
