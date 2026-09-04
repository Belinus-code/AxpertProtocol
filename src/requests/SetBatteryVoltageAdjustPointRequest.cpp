// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBatteryVoltageAdjustPointRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBatteryVoltageAdjustPointRequest::build(char* buf, size_t bufCapacity) const {
    if (point != 1 && point != 2) return 0;

    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, point == 1 ? "BTA1" : "BTA2")) return 0;
    if (!appendFixed(buf, bufCapacity, &offset, voltage, 2)) return 0;
    return offset;
}
