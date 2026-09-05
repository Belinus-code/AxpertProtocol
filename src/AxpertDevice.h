// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <Arduino.h>
#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <SoftwareSerial.h>
#endif

#include "AxpertConfig.h"
#include "AxpertTransport.h"

#include "responses/BatteryEqualizationStatusResponse.h"
#include "responses/ChargingCurrentOptionsResponse.h"
#include "responses/DefaultSettingsResponse.h"
#include "responses/DeviceModeResponse.h"
#include "responses/EnergyResponse.h"
#include "responses/FirmwareVersionResponse.h"
#include "responses/FlagStatusResponse.h"
#include "responses/GeneralStatusResponse.h"
#include "responses/ModelNameResponse.h"
#include "responses/ParallelInfoResponse.h"
#include "responses/ProtocolIdResponse.h"
#include "responses/RatingInfoResponse.h"
#include "responses/SerialNumberResponse.h"
#include "responses/TimeResponse.h"
#include "responses/WarningStatusResponse.h"

#include "requests/SetBatteryCutOffVoltageRequest.h"
#include "requests/SetBatteryCvChargingVoltageRequest.h"
#include "requests/SetBatteryEqualizationActiveRequest.h"
#include "requests/SetBatteryEqualizationEnabledRequest.h"
#include "requests/SetBatteryEqualizationOverTimeRequest.h"
#include "requests/SetBatteryEqualizationPeriodRequest.h"
#include "requests/SetBatteryEqualizationTimeRequest.h"
#include "requests/SetBatteryEqualizationVoltageRequest.h"
#include "requests/SetBatteryFloatChargingVoltageRequest.h"
#include "requests/SetBatteryTypeRequest.h"
#include "requests/SetBatteryVoltageAdjustPointRequest.h"
#include "requests/SetBatteryVoltageBackToBatteryRequest.h"
#include "requests/SetBatteryVoltageBackToUtilityRequest.h"
#include "requests/SetBmsMessageRequest.h"
#include "requests/SetChargerSourcePriorityRequest.h"
#include "requests/SetDataLogPeriodRequest.h"
#include "requests/SetFlagsRequest.h"
#include "requests/SetGridWorkingRangeRequest.h"
#include "requests/SetMaxChargingCurrentRequest.h"
#include "requests/SetMaxChargingTimeAtCvRequest.h"
#include "requests/SetMaxUtilityChargingCurrentRequest.h"
#include "requests/SetOutputFrequencyRequest.h"
#include "requests/SetOutputModeRequest.h"
#include "requests/SetOutputRatingVoltageRequest.h"
#include "requests/SetOutputSourcePriorityRequest.h"
#include "requests/SetParallelChargerSourcePriorityRequest.h"
#include "requests/SetPvOkConditionRequest.h"
#include "requests/SetPvPowerBalanceRequest.h"

// One instance per physical inverter/tracker. Holds no static/global state,
// so multiple instances (on different Stream ports) can be used side by
// side - see examples/MultipleInverters.
class AxpertDevice {
public:
    // Fully generic escape hatch: `port` can be anything Stream-derived
    // that this library doesn't know how to configure itself (a
    // SoftwareSerial you constructed yourself, a custom RS485 wrapper,
    // ...). You must call its begin()/equivalent yourself before using
    // this device - AxpertDevice::begin() is a no-op for this constructor,
    // since Stream itself has no begin() to call.
    explicit AxpertDevice(Stream& port, uint16_t defaultTimeoutMs = AXPERT_DEFAULT_TIMEOUT_MS);

    // Binds to a hardware UART (Serial1, Serial2, ...) on any
    // architecture. Call begin() from setup() to have it configured at
    // the protocol's required 2400 8N1 for you - don't call port.begin()
    // yourself, and don't call begin() from outside setup()/loop() (e.g.
    // not for a global AxpertDevice at namespace scope): touching hardware
    // before the Arduino runtime has initialized it is undefined on many
    // platforms, which is exactly why this constructor only records the
    // port and defers the actual begin() call.
    AxpertDevice(HardwareSerial& port, uint16_t defaultTimeoutMs = AXPERT_DEFAULT_TIMEOUT_MS);

    // Convenience default: binds to `Serial`. Same begin()-from-setup()
    // rule as the HardwareSerial& constructor above - this ties up the
    // port you might otherwise use for debug output on boards with only
    // one hardware UART (e.g. Uno); prefer Serial1/SoftwareSerial there.
    AxpertDevice();

#if defined(ARDUINO_ARCH_AVR)
    // Binds to a SoftwareSerial you constructed and own yourself. Only
    // available on AVR, where the Arduino core ships SoftwareSerial;
    // other architectures don't have a built-in equivalent (see the
    // rx/tx-pin constructor below for why). begin() configures it for you.
    AxpertDevice(SoftwareSerial& port, uint16_t defaultTimeoutMs = AXPERT_DEFAULT_TIMEOUT_MS);

    // Convenience overload for AVR boards without a spare hardware UART:
    // owns an internal SoftwareSerial on the given pins (allocated here,
    // freed in the destructor). begin() configures it for you.
    AxpertDevice(uint8_t rxPin, uint8_t txPin, uint16_t defaultTimeoutMs = AXPERT_DEFAULT_TIMEOUT_MS);

    ~AxpertDevice();
#endif

#if defined(ARDUINO_ARCH_ESP32)
    // ESP32-specific: remaps a hardware UART to arbitrary GPIO pins - the
    // "any RX/TX pins" equivalent of the AVR rx/tx-pin constructor, but
    // using a real hardware UART instead of software bit-banging (ESP32
    // has no built-in SoftwareSerial). `port` is typically Serial1 or
    // Serial2. begin() calls port.begin(2400, SERIAL_8N1, rxPin, txPin).
    AxpertDevice(HardwareSerial& port, int8_t rxPin, int8_t txPin, uint16_t defaultTimeoutMs = AXPERT_DEFAULT_TIMEOUT_MS);
#endif

    // Non-copyable: an instance may own a heap-allocated SoftwareSerial
    // (the AVR rx/tx-pin constructor) that a copy would end up
    // double-freeing.
    AxpertDevice(const AxpertDevice&) = delete;
    AxpertDevice& operator=(const AxpertDevice&) = delete;

    // Configures the underlying port at the protocol's required 2400 8N1,
    // for whichever constructor overload above was used to make this
    // instance knowledgeable enough to do so. Call this from setup(), not
    // at global/namespace scope (see the HardwareSerial& constructor's
    // comment for why). No-op for the generic Stream& constructor - you
    // must have already begin()'d that port yourself in that case.
    void begin();

    void setDefaultTimeout(uint16_t timeoutMs);
    uint16_t defaultTimeout() const;

    // Enables/disables CRC verification on received responses (enabled by
    // default). Every request this library sends is still framed with a
    // correct CRC regardless of this setting - this only controls whether
    // a reply with a bad/missing CRC gets rejected. Useful for debugging
    // against a device (or a captured log) with nonconformant CRC framing;
    // leave this enabled for normal use, since it's your only defense
    // against acting on a corrupted reply.
    void setVerifyCrcOnReceive(bool verify);
    bool verifyCrcOnReceive() const;

    // --- Raw / escape hatch ---
    // Sends `command` (a bare command string, e.g. "QPIGS" - no CRC, no
    // trailing <cr>) for anything this library doesn't have a typed
    // method for yet, or to replay/probe a command verbatim. On success,
    // `responseBuf` holds the reply content as a null-terminated string
    // (leading '(' included, any trailing CRC bytes stripped - the same
    // convention every Response::parse() expects) and the call returns
    // true; `responseBufCapacity` must include room for that terminator.
    //
    // `appendCRC` (default true) controls only the *outgoing* side:
    // true frames `command` exactly like every other method here (CRC +
    // <cr> appended, reply's CRC checked per setVerifyCrcOnReceive()).
    // false sends `command` followed by just a bare <cr>, with no CRC on
    // either side of the wire - the shape used by QT (2.19), the
    // protocol's one genuinely CRC-less command; use this for a command
    // you know doesn't carry CRC framing.
    bool sendRawCommand(const char* command, char* responseBuf, size_t responseBufCapacity,
                         bool appendCRC = true, int32_t timeoutMs = -1);

    // --- Inquiry commands (protocol section 2) ---
    // `timeoutMs < 0` (the default) means "use this device's default
    // timeout"; pass a non-negative value to override it for this call only.
    bool queryProtocolId(ProtocolIdResponse& out, int32_t timeoutMs = -1);                    // 2.1 QPI
    bool querySerialNumber(SerialNumberResponse& out, int32_t timeoutMs = -1);                // 2.2 QID
    bool querySerialNumberExtended(SerialNumberResponse& out, int32_t timeoutMs = -1);        // 2.3 QSID
    bool queryMainFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs = -1);      // 2.4 QVFW
    bool querySccFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs = -1);       // 2.5 QVFW2
    bool queryRemotePanelFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs = -1); // 2.6 QVFW3
    bool queryRatingInfo(RatingInfoResponse& out, int32_t timeoutMs = -1);                    // 2.8 QPIRI
    bool queryFlagStatus(FlagStatusResponse& out, int32_t timeoutMs = -1);                    // 2.9 QFLAG
    bool queryGeneralStatus(GeneralStatusResponse& out, int32_t timeoutMs = -1);              // 2.10 QPIGS
    bool queryDeviceMode(DeviceModeResponse& out, int32_t timeoutMs = -1);                    // 2.11 QMOD
    bool queryWarningStatus(WarningStatusResponse& out, int32_t timeoutMs = -1);              // 2.12 QPIWS
    bool queryDefaultSettings(DefaultSettingsResponse& out, int32_t timeoutMs = -1);          // 2.13 QDI
    bool queryMaxChargingCurrentOptions(ChargingCurrentOptionsResponse& out, int32_t timeoutMs = -1);        // 2.14 QMCHGCR
    bool queryMaxUtilityChargingCurrentOptions(ChargingCurrentOptionsResponse& out, int32_t timeoutMs = -1); // 2.15 QMUCHGCR

    // 2.19 QT. Special case: neither the request nor the reply carries a
    // CRC at all (see protocol 2.19) - the implementation must skip
    // AxpertCRC framing entirely for this one call.
    bool queryTime(TimeResponse& out, int32_t timeoutMs = -1);

    bool queryModelName(ModelNameResponse& out, int32_t timeoutMs = -1);                      // 2.20 QMN
    bool queryGeneralModelName(GeneralModelNameResponse& out, int32_t timeoutMs = -1);        // 2.21 QGMN
    bool queryBatteryEqualizationStatus(BatteryEqualizationStatusResponse& out, int32_t timeoutMs = -1); // 2.22 QBEQI

    // 2.23 QPGSn. `unitIndex` selects which unit in a parallel system to
    // query (n in "QPGSn").
    bool queryParallelInfo(uint8_t unitIndex, ParallelInfoResponse& out, int32_t timeoutMs = -1);

    bool queryTotalPvGeneratedEnergy(EnergyResponse& out, int32_t timeoutMs = -1);            // 2.25 QET
    bool queryPvGeneratedEnergyOfYear(uint16_t year, EnergyResponse& out, int32_t timeoutMs = -1); // 2.26 QEY
    bool queryPvGeneratedEnergyOfMonth(uint16_t year, uint8_t month, EnergyResponse& out, int32_t timeoutMs = -1); // 2.27 QEM
    bool queryPvGeneratedEnergyOfDay(uint16_t year, uint8_t month, uint8_t day, EnergyResponse& out, int32_t timeoutMs = -1); // 2.28 QED
    bool queryTotalOutputLoadEnergy(EnergyResponse& out, int32_t timeoutMs = -1);             // 2.29 QLT
    bool queryOutputLoadEnergyOfYear(uint16_t year, EnergyResponse& out, int32_t timeoutMs = -1); // 2.30 QLY
    bool queryOutputLoadEnergyOfMonth(uint16_t year, uint8_t month, EnergyResponse& out, int32_t timeoutMs = -1); // 2.31 QLM
    bool queryOutputLoadEnergyOfDay(uint16_t year, uint8_t month, uint8_t day, EnergyResponse& out, int32_t timeoutMs = -1); // 2.32 QLD

    // Not implemented on purpose - QBMS (2.34)'s reply format isn't
    // documented in the protocol PDF (no field table at all) and no public
    // reference for it was found either; implementing it would mean
    // guessing field meanings, which isn't worth the risk.
    //
    // Also not implemented on purpose - removed in this protocol revision:
    // VERFW (2.7), QOPPT/QCHPT/QOPCHT (2.16-2.18), Q2PGSn (2.24), QBATCD
    // (2.33), QLED (2.36).

    // --- Setting commands (protocol section 3) ---
    // Every setter below takes the matching Request struct from
    // src/requests/ and returns whether the device ACKed it (via the
    // shared AckResponse). Stateless commands (no configurable parameter)
    // don't need a request instance - their Request struct's build() is
    // static and gets called internally.
    bool startAteTest(int32_t timeoutMs = -1);                                        // 3.1 ATE1
    bool endAteTest(int32_t timeoutMs = -1);                                          // 3.2 ATE0

    // 3.3 PE/PD. Every one of SetFlagsRequest's 8 fields can independently
    // be Enabled/Disabled/left Unchanged - see that struct's comment. Note
    // this is the one setter that doesn't necessarily map to a single
    // physical transaction: it sends a PE command, a PD command, both, or
    // (if every flag is Unchanged) neither, per what the request actually
    // asks for.
    bool setFlags(const SetFlagsRequest& request, int32_t timeoutMs = -1);

    // Convenience wrappers around setFlags() for changing exactly one flag
    // without touching the other seven.
    bool setSilenceBuzzerEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setOverloadBypassEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setLcdEscapeToDefaultEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setOverloadRestartEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setOverTemperatureRestartEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setBacklightOnEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setAlarmOnPrimarySourceInterruptEnabled(bool enabled, int32_t timeoutMs = -1);
    bool setFaultCodeRecordEnabled(bool enabled, int32_t timeoutMs = -1);

    bool resetToDefaults(int32_t timeoutMs = -1);                                     // 3.4 PF
    bool setMaxChargingCurrent(const SetMaxChargingCurrentRequest& request, int32_t timeoutMs = -1);               // 3.5 MNCHGC
    bool setMaxUtilityChargingCurrent(const SetMaxUtilityChargingCurrentRequest& request, int32_t timeoutMs = -1); // 3.6 MUCHGC
    bool setOutputFrequency(const SetOutputFrequencyRequest& request, int32_t timeoutMs = -1);                     // 3.7 F
    bool setOutputRatingVoltage(const SetOutputRatingVoltageRequest& request, int32_t timeoutMs = -1);             // 3.8 POPV
    bool setOutputSourcePriority(const SetOutputSourcePriorityRequest& request, int32_t timeoutMs = -1);           // 3.9 POP
    bool setOutputMode(const SetOutputModeRequest& request, int32_t timeoutMs = -1);                               // 3.10 POPM
    bool setBatteryVoltageBackToUtility(const SetBatteryVoltageBackToUtilityRequest& request, int32_t timeoutMs = -1); // 3.12 PBCV
    bool setBatteryVoltageBackToBattery(const SetBatteryVoltageBackToBatteryRequest& request, int32_t timeoutMs = -1); // 3.13 PBDV
    bool setChargerSourcePriority(const SetChargerSourcePriorityRequest& request, int32_t timeoutMs = -1);         // 3.14 PCP
    bool setParallelChargerSourcePriority(const SetParallelChargerSourcePriorityRequest& request, int32_t timeoutMs = -1); // 3.15 PPCP
    bool setGridWorkingRange(const SetGridWorkingRangeRequest& request, int32_t timeoutMs = -1);                   // 3.16 PGR
    bool setBatteryType(const SetBatteryTypeRequest& request, int32_t timeoutMs = -1);                             // 3.17 PBT
    bool setBatteryCutOffVoltage(const SetBatteryCutOffVoltageRequest& request, int32_t timeoutMs = -1);           // 3.18 PSDV
    bool setBatteryCvChargingVoltage(const SetBatteryCvChargingVoltageRequest& request, int32_t timeoutMs = -1);   // 3.19 PCVV
    bool setBatteryFloatChargingVoltage(const SetBatteryFloatChargingVoltageRequest& request, int32_t timeoutMs = -1); // 3.20 PBFT
    bool setBatteryVoltageAdjustPoint(const SetBatteryVoltageAdjustPointRequest& request, int32_t timeoutMs = -1); // 3.21/3.22 BTA1/BTA2
    bool resetBatteryVoltageAdjust(int32_t timeoutMs = -1);                           // 3.23 BTA0
    bool setPvOkCondition(const SetPvOkConditionRequest& request, int32_t timeoutMs = -1);   // 3.24 PPVOKC
    bool setPvPowerBalance(const SetPvPowerBalanceRequest& request, int32_t timeoutMs = -1); // 3.25 PSPB
    bool resetEnergyData(int32_t timeoutMs = -1);                                     // 3.26 RTEY
    bool eraseDataLog(int32_t timeoutMs = -1);                                        // 3.27 RTDL
    bool setDataLogPeriod(const SetDataLogPeriodRequest& request, int32_t timeoutMs = -1);   // 3.28 PDL
    bool setBatteryEqualizationEnabled(const SetBatteryEqualizationEnabledRequest& request, int32_t timeoutMs = -1); // 3.29 PBEQE
    bool setBatteryEqualizationTime(const SetBatteryEqualizationTimeRequest& request, int32_t timeoutMs = -1);       // 3.30 PBEQT
    bool setBatteryEqualizationPeriod(const SetBatteryEqualizationPeriodRequest& request, int32_t timeoutMs = -1);   // 3.31 PBEQP
    bool setBatteryEqualizationVoltage(const SetBatteryEqualizationVoltageRequest& request, int32_t timeoutMs = -1); // 3.32 PBEQV
    bool setBatteryEqualizationOverTime(const SetBatteryEqualizationOverTimeRequest& request, int32_t timeoutMs = -1); // 3.33 PBEQOT
    bool setBatteryEqualizationActive(const SetBatteryEqualizationActiveRequest& request, int32_t timeoutMs = -1);   // 3.34 PBEQA
    bool setMaxChargingTimeAtCv(const SetMaxChargingTimeAtCvRequest& request, int32_t timeoutMs = -1);               // 3.35 PCVT
    bool setBmsMessage(const SetBmsMessageRequest& request, int32_t timeoutMs = -1);                                 // 2.35 PBMS

    // Not implemented on purpose - removed in this protocol revision:
    // POPLG (3.11), SRS (3.36), DAT (3.37), PBATMAXDISC (3.38), PBATCD
    // (3.39), PLEDE/PLEDS/PLEDM/PLEDB/PLEDT/PLEDC (3.40-3.45).

private:
    // Declared before _transport quite deliberately: member initializer
    // lists run in declaration order, not the order written, and the
    // rx/tx-pin AVR constructor needs to allocate a SoftwareSerial and use
    // it for both _softwareSerial and _transport's initializer.

    // Non-null if constructed from a HardwareSerial& (including the
    // default and ESP32 pin-remap constructors) - tells begin() it can
    // configure this port itself.
    HardwareSerial* _hardwareSerial = nullptr;

#if defined(ARDUINO_ARCH_AVR)
    // Non-null if constructed from a SoftwareSerial&, whether borrowed
    // (SoftwareSerial& constructor) or owned (rx/tx-pin constructor, see
    // _ownsSoftwareSerial).
    SoftwareSerial* _softwareSerial = nullptr;
    bool _ownsSoftwareSerial = false;
#endif

#if defined(ARDUINO_ARCH_ESP32)
    // Set by the pin-remap constructor; -1 means "use the port's default
    // pins" (plain begin(2400, SERIAL_8N1) instead of the 4-arg overload).
    int8_t _rxPin = -1;
    int8_t _txPin = -1;
#endif

    AxpertTransport _transport;
    uint16_t _defaultTimeoutMs;
    bool _verifyCrcOnReceive = true;

    // Resolves a per-call override (`timeoutMsOverride < 0` => use
    // `_defaultTimeoutMs`) into an actual millisecond value.
    uint16_t resolveTimeout(int32_t timeoutMsOverride) const;

    // The generic request/response plumbing every query*()/set*() method
    // is built on. See AxpertDevice.cpp for the full writeup of what each
    // one does - kept out of this header since none of it is templated in
    // a way other translation units would ever instantiate.
    bool transactRaw(const char* payload, size_t payloadLen, uint8_t* respBuf,
                      size_t respBufCapacity, size_t& outContentLen, int32_t timeoutMs);

    template <typename ResponseT>
    bool transact(const char* payload, size_t payloadLen, ResponseT& out, int32_t timeoutMs);

    template <typename ResponseT>
    bool transact(const char* payload, ResponseT& out, int32_t timeoutMs);

    template <typename RequestT>
    bool transactSet(const RequestT& request, int32_t timeoutMs);

    template <typename RequestT>
    bool transactSet(int32_t timeoutMs);
};
