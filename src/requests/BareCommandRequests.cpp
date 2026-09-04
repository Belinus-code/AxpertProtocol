// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "BareCommandRequests.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t StartAteTestRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "ATE1") ? offset : 0;
}

size_t EndAteTestRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "ATE0") ? offset : 0;
}

size_t ResetToDefaultsRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "PF") ? offset : 0;
}

size_t ResetBatteryVoltageAdjustRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "BTA0") ? offset : 0;
}

size_t ResetEnergyDataRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "RTEY") ? offset : 0;
}

size_t EraseDataLogRequest::build(char* buf, size_t bufCapacity) {
    size_t offset = 0;
    return appendText(buf, bufCapacity, &offset, "RTDL") ? offset : 0;
}
