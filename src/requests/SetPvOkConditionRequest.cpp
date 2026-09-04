// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetPvOkConditionRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetPvOkConditionRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PPVOKC")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, static_cast<uint32_t>(condition), 1)) return 0;
    return offset;
}
