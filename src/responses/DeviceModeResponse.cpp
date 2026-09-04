// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "DeviceModeResponse.h"

bool DeviceModeResponse::parse(const uint8_t* raw, size_t len, DeviceModeResponse& out) {
    // "(M" where M in {P,S,L,B,F,D,C,Y,E}.
    if (len != 2 || raw[0] != '(') return false;

    switch (static_cast<char>(raw[1])) {
        case 'P': out.mode = AxpertDeviceMode::PowerOn; break;
        case 'S': out.mode = AxpertDeviceMode::Standby; break;
        case 'L': out.mode = AxpertDeviceMode::Line; break;
        case 'B': out.mode = AxpertDeviceMode::Battery; break;
        case 'F': out.mode = AxpertDeviceMode::Fault; break;
        case 'D': out.mode = AxpertDeviceMode::Shutdown; break;
        case 'C': out.mode = AxpertDeviceMode::Charge; break;
        case 'Y': out.mode = AxpertDeviceMode::Bypass; break;
        case 'E': out.mode = AxpertDeviceMode::Eco; break;
        default: out.mode = AxpertDeviceMode::Unknown; break;
    }
    return true;
}
