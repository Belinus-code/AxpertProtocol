// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// PBMS (2.35): "PBMSa bbb c d e fff ggg hhh iiii jjjj" - relays BMS state
// to the device. Reply is just ACK/NAK like any setting command (see
// AckResponse). Voltage/current fields are documented as plain integers
// (no decimal point, unlike e.g. PBCV's "nn.n") - taken at face value here.
struct SetBmsMessageRequest {
    bool disconnectBattery;        // a: false = connect, true = disconnect
    uint16_t batteryPercent;       // bbb, %
    bool forceAcCharge;            // c: force AC charge in any case
    bool disableDischarge;         // d: battery stop discharge flag
    bool disableCharge;            // e: battery stop charge flag
    uint16_t cvChargingVoltage;    // fff, V
    uint16_t floatingChargingVoltage; // ggg, V
    uint16_t cutOffVoltage;        // hhh, V
    uint16_t maxChargingCurrent;   // iiii, A
    uint16_t maxDischargingCurrent; // jjjj, A

    size_t build(char* buf, size_t bufCapacity) const;
};
