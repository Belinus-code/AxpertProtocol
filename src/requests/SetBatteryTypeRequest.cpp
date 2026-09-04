// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBatteryTypeRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBatteryTypeRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PBT")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, static_cast<uint32_t>(type), 2)) return 0;
    return offset;
}
