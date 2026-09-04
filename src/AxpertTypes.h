// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stdint.h>

// Protocol 2.11 / 3.x: device operating mode (QMOD).
enum class AxpertDeviceMode : uint8_t {
    PowerOn,
    Standby,
    Line,
    Battery,
    Fault,
    Shutdown,
    Charge,
    Bypass,
    Eco,
    Unknown
};

// Protocol 2.8 / 3.9: output source priority.
enum class AxpertOutputSourcePriority : uint8_t {
    UtilityFirst = 0,
    SolarFirst = 1,
    SbuFirst = 2
};

// Protocol 2.8 / 3.14: charger source priority.
enum class AxpertChargerSourcePriority : uint8_t {
    UtilityFirst = 0,
    SolarFirst = 1,
    SolarAndUtility = 2,
    SolarOnly = 3
};

// Protocol 2.8 / 3.17: battery type.
enum class AxpertBatteryType : uint8_t {
    Agm = 0,
    Flooded = 1,
    User = 2,
    Pylontech = 3,
    Shinheung = 4,
    Weco = 5,
    Soltaro = 6
};

// Protocol 2.8: AC input voltage range.
enum class AxpertInputVoltageRange : uint8_t {
    Appliance = 0,
    Ups = 1
};

// Protocol 2.8 / 3.10: output mode.
enum class AxpertOutputMode : uint8_t {
    SingleMachine = 0,
    Parallel = 1,
    Phase1Of3 = 2,
    Phase2Of3 = 3,
    Phase3Of3 = 4
};

// Protocol 2.8: machine type (QPIRI field SS). Wire values are the literal
// 2-digit codes ("00"/"01"/"10"), which is why Hybrid == 10 and not 2.
enum class AxpertMachineType : uint8_t {
    GridTie = 0,
    OffGrid = 1,
    Hybrid = 10
};

// Protocol 2.8: topology (QPIRI field T).
enum class AxpertTopology : uint8_t {
    Transformerless = 0,
    Transformer = 1
};

// Protocol 2.8 / 3.24 (PPVOKC): PV OK condition for parallel systems.
enum class AxpertPvOkConditionForParallel : uint8_t {
    AnyUnitConnected = 0,
    AllUnitsConnected = 1
};

// Protocol 2.8 / 3.25 (PSPB): solar power balance.
enum class AxpertPvPowerBalance : uint8_t {
    MaxCurrentIsMaxChargedCurrent = 0,
    MaxPowerIsChargedPlusLoadPower = 1
};

// Protocol 2.10 (QPIGS bits b2b1b0) / 2.23 (QPGSn charging-related bits):
// combined charging source status. Only the 4 documented bit patterns are
// valid - values are the literal 3-bit patterns, not sequential.
enum class AxpertChargingSourceStatus : uint8_t {
    None = 0b000,
    SccOnly = 0b110,
    AcOnly = 0b101,
    SccAndAc = 0b111
};

// Protocol 2.23 (QPGSn bits b4b3): per-unit battery status in a parallel
// system.
enum class AxpertParallelBatteryStatus : uint8_t {
    Normal = 0,
    Under = 1,
    Open = 2
};

// Protocol 2.12 / 2.13: fault codes as reported by QPGSn's fault-code field
// and the fault-code table (only some values are used - gaps are
// intentional, not missing entries).
enum class AxpertFaultCode : uint8_t {
    None = 0, // QPGSn's fault-code field reports 0 when there is no fault
    FanLocked = 1,
    OverTemperature = 2,
    BatteryVoltageTooHigh = 3,
    BatteryVoltageTooLow = 4,
    OutputShortCircuitedOrOverTemperature = 5,
    OutputVoltageTooHigh = 6,
    OverLoadTimeOut = 7,
    BusVoltageTooHigh = 8,
    BusSoftStartFailed = 9,
    PvChargerCurrentOver = 10,
    PvOverVolt = 11,
    DcDcOverCurrent = 12,
    OverCurrentInverter = 51,
    InverterSoftStartFailed = 53,
    OverDcVoltageOnOutputOfInverter = 55,
    CurrentSensorFailed = 57,
    OutputVoltageTooLow = 58,
    PvVoltageHigh = 59,
    InverterNegativePower = 60, // KING only
    ParallelVersionDifferent = 71,
    OutputCircuitFailed = 72,
    OutputVoltDifferent = 73,
    CanCommunicationFailed = 80,
    ParallelHostLineLost = 81,
    ParallelSynchronizedSignalLost = 82,
    ParallelBatteryVoltageDetectDifferent = 83,
    ParallelLineVoltageOrFrequencyDetectDifferent = 84,
    ParallelOutputSettingDifferent = 86
};

// Protocol 2.12: bit indices into WarningStatusResponse::bits (QPIWS). Most
// bits are "fault if a1 is also set, warning otherwise" per the protocol -
// see the comment on each entry that calls this out.
//
// a24, a25, a28, a29 were previously named/classified faults or warnings
// but the protocol's revision history struck out both their description
// and their Fault/Warning classification, replacing the description with
// just "(Reserved)" - i.e. these bits no longer have a defined meaning.
// a30's classification ("Fault, only for KING") was struck with no
// replacement given, so its current classification is unspecified.
// a31, a34, a35 were struck out entirely (old VMIII-only meanings removed,
// no replacement given). a32/a33's old VMIII meaning was struck and
// replaced by a note that they now jointly form a fault code, only for
// MKSIII - exact bit layout of that fault code isn't spelled out further,
// so treat these two as reserved bits and verify against a real MKSIII
// device before relying on them.
enum class AxpertWarningBit : uint8_t {
    PvLoss = 0,                    // Reserved; Warning
    InverterFault = 1,
    BusOver = 2,
    BusUnder = 3,
    BusSoftFail = 4,
    LineFail = 5,
    OutputShort = 6,                // protocol: "OPVShort"; Fault
    InverterVoltageTooLow = 7,
    InverterVoltageTooHigh = 8,
    OverTemperature = 9,            // Fault if a1 set, else Warning
    FanLocked = 10,                 // Fault if a1 set, else Warning
    BatteryVoltageHigh = 11,        // Fault if a1 set, else Warning
    BatteryLowAlarm = 12,
    Reserved13 = 13,
    BatteryUnderShutdown = 14,
    BatteryDerating = 15,
    OverLoad = 16,                  // Fault if a1 set, else Warning
    EepromFault = 17,
    InverterOverCurrent = 18,
    InverterSoftFail = 19,
    SelfTestFail = 20,
    OutputDcVoltageOver = 21,
    BatteryOpen = 22,
    CurrentSensorFail = 23,
    Reserved24 = 24,                // formerly "Battery Short"; no longer classified
    Reserved25 = 25,                // formerly "Power limit"; no longer classified
    PvVoltageHigh = 26,             // Warning/Fault, all models
    PvOverCurrent = 27,             // Fault, all models; formerly "MPPT overload fault", KING only
    Reserved28 = 28,                // Warning; formerly "MPPT overload warning", KING only
    Reserved29 = 29,                // Warning; formerly "Battery too low to charge", KING only
    DcDcOverCurrent = 30,           // classification unspecified (previously "Fault, only for KING")
    Reserved31 = 31,
    MksiiiFaultCodeBit0 = 32,       // with MksiiiFaultCodeBit1, forms a fault code (MKSIII only) - verify against real hardware
    MksiiiFaultCodeBit1 = 33,
    Reserved34 = 34,
    Reserved35 = 35
};
