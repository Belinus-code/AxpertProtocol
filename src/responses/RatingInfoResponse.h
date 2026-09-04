// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../AxpertTypes.h"

// QPIRI (2.8): device rating information.
struct RatingInfoResponse {
    float acOutputRatingVoltage;          // BBB.B
    float acOutputRatingCurrent;          // CC.C
    float acOutputRatingVoltage2;         // DDD.D (protocol repeats this field)
    float acOutputRatingFrequency;        // EE.E
    float acOutputRatingCurrent2;         // FF.F
    uint16_t acOutputRatingApparentPower; // HHHH (VA)
    uint16_t acOutputRatingActivePower;   // IIII (W)
    float batteryRatingVoltage;           // JJ.J
    float batteryRechargeVoltage;         // KK.K
    float batteryUnderVoltage;            // JJ.J
    float batteryBulkVoltage;             // KK.K
    float batteryFloatVoltage;            // LL.L
    AxpertBatteryType batteryType;        // O
    uint16_t maxAcChargingCurrent;        // PPP
    uint16_t maxChargingCurrent;          // QQQ
    AxpertInputVoltageRange inputVoltageRange;         // O
    AxpertOutputSourcePriority outputSourcePriority;   // P
    AxpertChargerSourcePriority chargerSourcePriority; // Q
    uint8_t parallelMaxNumber;             // R
    AxpertMachineType machineType;         // SS
    AxpertTopology topology;               // T
    AxpertOutputMode outputMode;           // U
    float batteryRedischargeVoltage;       // VV.V
    AxpertPvOkConditionForParallel pvOkConditionForParallel; // W
    AxpertPvPowerBalance pvPowerBalance;   // X
    uint16_t maxChargingTimeAtCvMinutes;   // YYY

    static bool parse(const uint8_t* raw, size_t len, RatingInfoResponse& out);
};
