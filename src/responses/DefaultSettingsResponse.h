// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../AxpertTypes.h"

// QDI (2.13): the device's default setting values.
struct DefaultSettingsResponse {
    float acOutputVoltage;
    float acOutputFrequency;
    uint16_t maxAcChargingCurrent;
    float batteryUnderVoltage;
    float chargingFloatVoltage;
    float chargingBulkVoltage;
    float batteryRechargeVoltage;
    uint16_t maxChargingCurrent;
    AxpertInputVoltageRange inputVoltageRange;
    AxpertOutputSourcePriority outputSourcePriority;
    AxpertChargerSourcePriority chargerSourcePriority;
    AxpertBatteryType batteryType;
    bool buzzerEnabled;
    bool powerSavingEnabled;
    bool overloadRestartEnabled;
    bool overTemperatureRestartEnabled;
    bool lcdBacklightEnabled;
    bool alarmOnPrimarySourceInterruptEnabled;
    bool faultCodeRecordEnabled;
    bool overloadBypassEnabled;
    bool lcdEscapeToDefaultEnabled;
    AxpertOutputMode outputMode;
    float batteryRedischargeVoltage;
    AxpertPvOkConditionForParallel pvOkConditionForParallel;
    AxpertPvPowerBalance pvPowerBalance;
    uint16_t maxChargingTimeAtCvMinutes;

    static bool parse(const uint8_t* raw, size_t len, DefaultSettingsResponse& out);
};
