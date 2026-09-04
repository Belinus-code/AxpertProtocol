// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetBmsMessageRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetBmsMessageRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, "PBMS")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, disconnectBattery ? 1 : 0, 1)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, batteryPercent, 3)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, forceAcCharge ? 1 : 0, 1)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, disableDischarge ? 1 : 0, 1)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, disableCharge ? 1 : 0, 1)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, cvChargingVoltage, 3)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, floatingChargingVoltage, 3)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, cutOffVoltage, 3)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, maxChargingCurrent, 4)) return 0;
    if (!appendText(buf, bufCapacity, &offset, " ")) return 0;
    if (!appendUInt(buf, bufCapacity, &offset, maxDischargingCurrent, 4)) return 0;
    return offset;
}
