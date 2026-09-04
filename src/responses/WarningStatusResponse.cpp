// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "WarningStatusResponse.h"

bool WarningStatusResponse::parse(const uint8_t* raw, size_t len, WarningStatusResponse& out) {
    if (len != 37 || raw[0] != '(') return false; // '(' + 36 bit characters
    const char* text = reinterpret_cast<const char*>(raw) + 1;

    uint8_t* const fields[36] = {
        &out.pvLoss, &out.inverterFault, &out.busOver, &out.busUnder,
        &out.busSoftFail, &out.lineFail, &out.outputShort,
        &out.inverterVoltageTooLow, &out.inverterVoltageTooHigh,
        &out.overTemperature, &out.fanLocked, &out.batteryVoltageHigh,
        &out.batteryLowAlarm, &out.reserved13, &out.batteryUnderShutdown,
        &out.batteryDerating, &out.overLoad, &out.eepromFault,
        &out.inverterOverCurrent, &out.inverterSoftFail, &out.selfTestFail,
        &out.outputDcVoltageOver, &out.batteryOpen, &out.currentSensorFail,
        &out.reserved24, &out.reserved25, &out.pvVoltageHigh,
        &out.pvOverCurrent, &out.reserved28, &out.reserved29,
        &out.dcDcOverCurrent, &out.reserved31, &out.mksiiiFaultCodeBit0,
        &out.mksiiiFaultCodeBit1, &out.reserved34, &out.reserved35};

    for (size_t i = 0; i < 36; ++i) {
        char c = text[i];
        if (c != '0' && c != '1') return false;
        *fields[i] = static_cast<uint8_t>(c - '0');
    }
    return true;
}
