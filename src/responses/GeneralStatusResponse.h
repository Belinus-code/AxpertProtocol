// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QPIGS (2.10): device general status parameters. The most frequently
// polled command - see protocol 2.10 for the full field layout/order this
// struct mirrors.
struct GeneralStatusResponse {
    float gridVoltage;
    float gridFrequency;
    float acOutputVoltage;
    float acOutputFrequency;
    uint16_t acOutputApparentPower;
    uint16_t acOutputActivePower;
    uint8_t outputLoadPercent;
    uint16_t busVoltage;
    float batteryVoltage;
    uint16_t batteryChargingCurrent;
    uint8_t batteryCapacityPercent;
    uint16_t inverterHeatSinkTemperature;
    float pvInputCurrent;
    float pvInputVoltage;
    float batteryVoltageFromScc;
    uint16_t batteryDischargeCurrent;

    // Device status bits (b7..b0)
    uint8_t isPVFeedingLoad;
    uint8_t isConfigurationChanged;
    uint8_t isSccFirmwareUpdated;
    uint8_t isLoadOn;
    uint8_t isCharging;
    uint8_t isSCCCharging;
    uint8_t isACCharging;


    uint8_t batteryVoltageOffsetForFans10mV;
    uint8_t eepromVersion;
    uint32_t pvChargingPower;

    // Extended status bits (b10..b8)
    uint8_t isFloatingMode;
    uint8_t isSwitchOn;
    uint8_t isDustproofInstalled;

    static bool parse(const uint8_t* raw, size_t len, GeneralStatusResponse& out);
};
