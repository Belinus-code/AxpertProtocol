// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetFlagsRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

size_t SetFlagsRequest::build(char* buf, size_t bufCapacity) const {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, enable ? "PE" : "PD")) return 0;

    size_t before = offset;
    if (silenceBuzzer && !appendText(buf, bufCapacity, &offset, "a")) return 0;
    if (overloadBypass && !appendText(buf, bufCapacity, &offset, "b")) return 0;
    if (lcdEscapeToDefault && !appendText(buf, bufCapacity, &offset, "k")) return 0;
    if (overloadRestart && !appendText(buf, bufCapacity, &offset, "u")) return 0;
    if (overTemperatureRestart && !appendText(buf, bufCapacity, &offset, "v")) return 0;
    if (backlightOn && !appendText(buf, bufCapacity, &offset, "x")) return 0;
    if (alarmOnPrimarySourceInterrupt && !appendText(buf, bufCapacity, &offset, "y")) return 0;
    if (faultCodeRecord && !appendText(buf, bufCapacity, &offset, "z")) return 0;

    return offset > before ? offset : 0; // require at least one flag
}
