// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../AxpertTypes.h"

// QPGSn (2.23): parallel information inquiry for one unit in a parallel
// system. `n` (which unit to query) is a request-time parameter, not part
// of the response.
struct ParallelInfoResponse {
    bool exists;                  // A
    char serialNumber[15];        // BBBBBBBBBBBBBB (14 chars, null-terminated)
    AxpertDeviceMode workMode;    // C
    AxpertFaultCode faultCode;    // DD
    float gridVoltage;            // EEE.E
    float gridFrequency;          // FF.FF
    float acOutputVoltage;        // GGG.G
    float acOutputFrequency;      // HH.HH
    uint16_t acOutputApparentPower; // IIII, VA
    uint16_t acOutputActivePower;   // JJJJ, W
    uint8_t loadPercent;          // KKK
    float batteryVoltage;         // LL.L
    uint16_t batteryChargingCurrent; // MMM, A
    uint8_t batteryCapacityPercent;  // NNN
    float pvInputVoltage;         // OOO.O
    uint16_t totalChargingCurrent;   // PPP, A
    uint32_t totalAcOutputApparentPower; // QQQQQ, VA
    uint32_t totalOutputActivePower;     // RRRRR, W
    uint8_t totalAcOutputPercent; // SSS

    // Inverter status bits (b7..b0).
    uint8_t isSccOk;                 // b7: 1 SCC OK, 0 SCC loss
    uint8_t isAcCharging;            // b6
    uint8_t isSccCharging;           // b5
    AxpertParallelBatteryStatus batteryStatus; // b4b3
    uint8_t isLineLoss;              // b2
    uint8_t isLoadOn;                // b1
    uint8_t isConfigurationChanged;  // b0

    AxpertOutputMode outputMode;                       // T
    AxpertChargerSourcePriority chargerSourcePriority; // U
    uint16_t maxChargerCurrent;      // VVV, A
    uint16_t maxChargerRange;        // WWW, A
    uint16_t maxAcChargerCurrent;    // ZZZ, A
    uint8_t pvInputCurrent;          // XX, A
    uint16_t batteryDischargeCurrent; // YYY, A

    static bool parse(const uint8_t* raw, size_t len, ParallelInfoResponse& out);
};
