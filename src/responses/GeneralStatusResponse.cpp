// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "GeneralStatusResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool GeneralStatusResponse::parse(const uint8_t* raw, size_t len, GeneralStatusResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.gridVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.gridFrequency = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputFrequency = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputApparentPower = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.acOutputActivePower = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.outputLoadPercent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.busVoltage = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryCapacityPercent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.inverterHeatSinkTemperature = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvInputCurrent = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvInputVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryVoltageFromScc = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryDischargeCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    // b7b6b5b4b3b2b1b0 - b3 is reserved and intentionally not exposed.
    if (!nextToken(cursor, end, tok, tokLen) || tokLen != 8) return false;
    out.isPVFeedingLoad = static_cast<uint8_t>(tok[0] - '0');
    out.isConfigurationChanged = static_cast<uint8_t>(tok[1] - '0');
    out.isSccFirmwareUpdated = static_cast<uint8_t>(tok[2] - '0');
    out.isLoadOn = static_cast<uint8_t>(tok[3] - '0');
    out.isCharging = static_cast<uint8_t>(tok[5] - '0');
    out.isSCCCharging = static_cast<uint8_t>(tok[6] - '0');
    out.isACCharging = static_cast<uint8_t>(tok[7] - '0');

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryVoltageOffsetForFans10mV = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.eepromVersion = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvChargingPower = parseUIntToken(tok, tokLen);

    // b10b9b8
    if (!nextToken(cursor, end, tok, tokLen) || tokLen != 3) return false;
    out.isFloatingMode = static_cast<uint8_t>(tok[0] - '0');
    out.isSwitchOn = static_cast<uint8_t>(tok[1] - '0');
    out.isDustproofInstalled = static_cast<uint8_t>(tok[2] - '0');

    return true;
}
