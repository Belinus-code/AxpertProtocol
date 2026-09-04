// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBatteryEqualizationVoltageRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBatteryEqualizationVoltageRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PBEQV")) return 0;
    if (!appendFixed(buf, bufCapacity, &offset, voltage, 2)) return 0;
    return offset;
}
