// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "RatingInfoResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool RatingInfoResponse::parse(const uint8_t* raw, size_t len, RatingInfoResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingCurrent = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingVoltage2 = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingFrequency = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingCurrent2 = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingApparentPower = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputRatingActivePower = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryRatingVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryRechargeVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryUnderVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryBulkVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryFloatVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryType = static_cast<AxpertBatteryType>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxAcChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.inputVoltageRange = static_cast<AxpertInputVoltageRange>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.outputSourcePriority = static_cast<AxpertOutputSourcePriority>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.chargerSourcePriority = static_cast<AxpertChargerSourcePriority>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.parallelMaxNumber = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.machineType = static_cast<AxpertMachineType>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.topology = static_cast<AxpertTopology>(parseUIntToken(tok, tokLen));

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
