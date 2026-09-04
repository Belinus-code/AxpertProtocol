// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "DefaultSettingsResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool DefaultSettingsResponse::parse(const uint8_t* raw, size_t len, DefaultSettingsResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputFrequency = parseFloatToken(tok, tokLen);

    // "00DD" - leading zeros don't affect decimal parsing.
    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxAcChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryUnderVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.chargingFloatVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.chargingBulkVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryRechargeVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.inputVoltageRange = static_cast<AxpertInputVoltageRange>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.outputSourcePriority = static_cast<AxpertOutputSourcePriority>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.chargerSourcePriority = static_cast<AxpertChargerSourcePriority>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryType = static_cast<AxpertBatteryType>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.buzzerEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.powerSavingEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.overloadRestartEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.overTemperatureRestartEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.lcdBacklightEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.alarmOnPrimarySourceInterruptEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.faultCodeRecordEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.overloadBypassEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.lcdEscapeToDefaultEnabled = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.outputMode = static_cast<AxpertOutputMode>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryRedischargeVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvOkConditionForParallel = static_cast<AxpertPvOkConditionForParallel>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvPowerBalance = static_cast<AxpertPvPowerBalance>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxChargingTimeAtCvMinutes = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    return true;
}
