// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "ParallelInfoResponse.h"

#include "../AxpertParsing.h"

using namespace AxpertParsing;

bool ParallelInfoResponse::parse(const uint8_t* raw, size_t len, ParallelInfoResponse& out) {
    if (len == 0 || raw[0] != '(') return false;

    const char* cursor = reinterpret_cast<const char*>(raw) + 1; // skip '('
    const char* end = reinterpret_cast<const char*>(raw) + len;
    const char* tok;
    size_t tokLen;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.exists = parseUIntToken(tok, tokLen) != 0;

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    if (!copyToken(tok, tokLen, out.serialNumber, sizeof(out.serialNumber))) return false;

    if (!nextToken(cursor, end, tok, tokLen) || tokLen != 1) return false;
    switch (tok[0]) {
        case 'P': out.workMode = AxpertDeviceMode::PowerOn; break;
        case 'S': out.workMode = AxpertDeviceMode::Standby; break;
        case 'L': out.workMode = AxpertDeviceMode::Line; break;
        case 'B': out.workMode = AxpertDeviceMode::Battery; break;
        case 'F': out.workMode = AxpertDeviceMode::Fault; break;
        case 'D': out.workMode = AxpertDeviceMode::Shutdown; break;
        case 'C': out.workMode = AxpertDeviceMode::Charge; break;
        case 'Y': out.workMode = AxpertDeviceMode::Bypass; break;
        case 'E': out.workMode = AxpertDeviceMode::Eco; break;
        default: out.workMode = AxpertDeviceMode::Unknown; break;
    }

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.faultCode = static_cast<AxpertFaultCode>(parseUIntToken(tok, tokLen));

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
    out.loadPercent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryCapacityPercent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvInputVoltage = parseFloatToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.totalChargingCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.totalAcOutputApparentPower = parseUIntToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.totalOutputActivePower = parseUIntToken(tok, tokLen);

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.totalAcOutputPercent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    // b7b6b5b4b3b2b1b0
    if (!nextToken(cursor, end, tok, tokLen) || tokLen != 8) return false;
    out.isSccOk = static_cast<uint8_t>(tok[0] - '0');
    out.isAcCharging = static_cast<uint8_t>(tok[1] - '0');
    out.isSccCharging = static_cast<uint8_t>(tok[2] - '0');
    {
        uint8_t b4 = static_cast<uint8_t>(tok[3] - '0');
        uint8_t b3 = static_cast<uint8_t>(tok[4] - '0');
        out.batteryStatus = static_cast<AxpertParallelBatteryStatus>((b4 << 1) | b3);
    }
    out.isLineLoss = static_cast<uint8_t>(tok[5] - '0');
    out.isLoadOn = static_cast<uint8_t>(tok[6] - '0');
    out.isConfigurationChanged = static_cast<uint8_t>(tok[7] - '0');

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.outputMode = static_cast<AxpertOutputMode>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.chargerSourcePriority = static_cast<AxpertChargerSourcePriority>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxChargerCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxChargerRange = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.maxAcChargerCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.pvInputCurrent = static_cast<uint8_t>(parseUIntToken(tok, tokLen));

    if (!nextToken(cursor, end, tok, tokLen)) return false;
    out.batteryDischargeCurrent = static_cast<uint16_t>(parseUIntToken(tok, tokLen));

    return true;
}
