// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "SetFlagsRequest.h"

#include "../AxpertBuilding.h"

using namespace AxpertBuilding;

namespace {

size_t buildForState(const SetFlagsRequest& request, AxpertFlagState wanted, const char* command,
                      char* buf, size_t bufCapacity) {
    size_t offset = 0;
    if (!appendText(buf, bufCapacity, &offset, command)) return 0;

    size_t before = offset;
    if (request.silenceBuzzer == wanted && !appendText(buf, bufCapacity, &offset, "a")) return 0;
    if (request.overloadBypass == wanted && !appendText(buf, bufCapacity, &offset, "b")) return 0;
    if (request.lcdEscapeToDefault == wanted && !appendText(buf, bufCapacity, &offset, "k")) return 0;
    if (request.overloadRestart == wanted && !appendText(buf, bufCapacity, &offset, "u")) return 0;
    if (request.overTemperatureRestart == wanted && !appendText(buf, bufCapacity, &offset, "v")) return 0;
    if (request.backlightOn == wanted && !appendText(buf, bufCapacity, &offset, "x")) return 0;
    if (request.alarmOnPrimarySourceInterrupt == wanted && !appendText(buf, bufCapacity, &offset, "y")) return 0;
    if (request.faultCodeRecord == wanted && !appendText(buf, bufCapacity, &offset, "z")) return 0;

    return offset > before ? offset : 0; // require at least one flag in this state
}

} // namespace

size_t SetFlagsRequest::buildEnableCommand(char* buf, size_t bufCapacity) const {
    return buildForState(*this, AxpertFlagState::Enabled, "PE", buf, bufCapacity);
}

size_t SetFlagsRequest::buildDisableCommand(char* buf, size_t bufCapacity) const {
    return buildForState(*this, AxpertFlagState::Disabled, "PD", buf, bufCapacity);
}
