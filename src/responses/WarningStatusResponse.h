// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QPIWS (2.12): "(a0a1...a34a35" - 36 warning/fault bits, one per byte
// here. See AxpertWarningBit in AxpertTypes.h for what each bit means and
// which ones are deprecated/reserved in this protocol revision - the field
// names below follow that enum's naming.
struct WarningStatusResponse {
    uint8_t pvLoss;                 // a0
    uint8_t inverterFault;          // a1
    uint8_t busOver;                // a2
    uint8_t busUnder;                // a3
    uint8_t busSoftFail;            // a4
    uint8_t lineFail;               // a5
    uint8_t outputShort;            // a6
    uint8_t inverterVoltageTooLow;  // a7
    uint8_t inverterVoltageTooHigh; // a8
    uint8_t overTemperature;        // a9
    uint8_t fanLocked;              // a10
    uint8_t batteryVoltageHigh;     // a11
    uint8_t batteryLowAlarm;        // a12
    uint8_t reserved13;             // a13
    uint8_t batteryUnderShutdown;   // a14
    uint8_t batteryDerating;        // a15
    uint8_t overLoad;               // a16
    uint8_t eepromFault;            // a17
    uint8_t inverterOverCurrent;    // a18
    uint8_t inverterSoftFail;       // a19
    uint8_t selfTestFail;           // a20
    uint8_t outputDcVoltageOver;    // a21
    uint8_t batteryOpen;            // a22
    uint8_t currentSensorFail;      // a23
    uint8_t reserved24;             // a24
    uint8_t reserved25;             // a25
    uint8_t pvVoltageHigh;          // a26
    uint8_t pvOverCurrent;          // a27
    uint8_t reserved28;             // a28
    uint8_t reserved29;             // a29
    uint8_t dcDcOverCurrent;        // a30
    uint8_t reserved31;             // a31
    uint8_t mksiiiFaultCodeBit0;    // a32
    uint8_t mksiiiFaultCodeBit1;    // a33
    uint8_t reserved34;             // a34
    uint8_t reserved35;             // a35

    static bool parse(const uint8_t* raw, size_t len, WarningStatusResponse& out);
};
