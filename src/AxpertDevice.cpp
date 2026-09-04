// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AxpertDevice.h"

#include <string.h>

#include "AxpertBuilding.h"
#include "AxpertCRC.h"
#include "responses/AckResponse.h"
#include "requests/BareCommandRequests.h"

AxpertDevice::AxpertDevice(Stream& port, uint16_t defaultTimeoutMs)
    : _transport(port), _defaultTimeoutMs(defaultTimeoutMs) {}

AxpertDevice::AxpertDevice(HardwareSerial& port, uint16_t defaultTimeoutMs)
    : _hardwareSerial(&port), _transport(port), _defaultTimeoutMs(defaultTimeoutMs) {}

AxpertDevice::AxpertDevice()
    : AxpertDevice(Serial, AXPERT_DEFAULT_TIMEOUT_MS) {}

#if defined(ARDUINO_ARCH_AVR)
AxpertDevice::AxpertDevice(SoftwareSerial& port, uint16_t defaultTimeoutMs)
    : _softwareSerial(&port), _ownsSoftwareSerial(false), _transport(port), _defaultTimeoutMs(defaultTimeoutMs) {}

AxpertDevice::AxpertDevice(uint8_t rxPin, uint8_t txPin, uint16_t defaultTimeoutMs)
    : _softwareSerial(new SoftwareSerial(rxPin, txPin)),
      _ownsSoftwareSerial(true),
      _transport(*_softwareSerial),
      _defaultTimeoutMs(defaultTimeoutMs) {}

AxpertDevice::~AxpertDevice() {
    if (_ownsSoftwareSerial) {
        // SoftwareSerial is polymorphic (via Stream's virtual methods) but
        // has a non-virtual destructor, which trips -Wdelete-non-virtual-dtor
        // here. It's a false positive for this exact call: we only ever
        // delete through the same SoftwareSerial* we allocated with `new
        // SoftwareSerial(...)` above, never through a Stream*/base
        // pointer, so static and dynamic type match and this is
        // well-defined regardless of the missing virtual destructor.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
        delete _softwareSerial;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
    }
}
#endif

#if defined(ARDUINO_ARCH_ESP32)
AxpertDevice::AxpertDevice(HardwareSerial& port, int8_t rxPin, int8_t txPin, uint16_t defaultTimeoutMs)
    : _hardwareSerial(&port), _rxPin(rxPin), _txPin(txPin), _transport(port), _defaultTimeoutMs(defaultTimeoutMs) {}
#endif

void AxpertDevice::begin() {
    if (_hardwareSerial) {
#if defined(ARDUINO_ARCH_ESP32)
        if (_rxPin >= 0 && _txPin >= 0) {
            _hardwareSerial->begin(2400, SERIAL_8N1, _rxPin, _txPin);
            return;
        }
#endif
        _hardwareSerial->begin(2400, SERIAL_8N1);
        return;
    }
#if defined(ARDUINO_ARCH_AVR)
    if (_softwareSerial) {
        _softwareSerial->begin(2400); // SoftwareSerial has no config param - always 8N1
        return;
    }
#endif
    // Generic Stream& constructor: nothing we can configure ourselves -
    // the caller must have already begin()'d the port before constructing
    // this device.
}

void AxpertDevice::setDefaultTimeout(uint16_t timeoutMs) {
    _defaultTimeoutMs = timeoutMs;
}

uint16_t AxpertDevice::defaultTimeout() const {
    return _defaultTimeoutMs;
}

void AxpertDevice::setVerifyCrcOnReceive(bool verify) {
    _verifyCrcOnReceive = verify;
}

bool AxpertDevice::verifyCrcOnReceive() const {
    return _verifyCrcOnReceive;
}

uint16_t AxpertDevice::resolveTimeout(int32_t timeoutMsOverride) const {
    return (timeoutMsOverride < 0) ? _defaultTimeoutMs : static_cast<uint16_t>(timeoutMsOverride);
}

// ---------------------------------------------------------------------------
// Generic request/response plumbing
//
// Every query*()/set*() method below is just:
//   1. produce a payload string (a literal, a hand-built one via
//      AxpertBuilding, or a Request::build()),
//   2. hand it to transact() or transactSet(),
//   3. done.
//
// transactRaw() does the actual work: frame the payload with
// AxpertCRC::appendFramed(), send it and read the reply via _transport,
// verify the reply's CRC with AxpertCRC::verify() (unless
// setVerifyCrcOnReceive(false) turned that off), and hand back the
// parseable content (leading '(' included, CRC stripped either way - the
// convention every Response::parse() expects). transact<ResponseT>() is a thin
// wrapper for the common case of parsing the reply via `ResponseT::parse`;
// it's kept separate from transactRaw() because a couple of commands
// (querySerialNumberExtended, queryTime) need something other than the
// plain "call ::parse" shape. transactSet<RequestT>() covers every setting
// command: build the request, transact it, and check the resulting
// AckResponse.
// ---------------------------------------------------------------------------

bool AxpertDevice::transactRaw(const char* payload, size_t payloadLen, uint8_t* respBuf,
                                size_t respBufCapacity, size_t& outContentLen, int32_t timeoutMs) {
    if (payloadLen == 0) return false;

    uint8_t frame[AXPERT_MAX_FRAME_LEN];
    if (payloadLen > sizeof(frame)) return false;
    memcpy(frame, payload, payloadLen);

    size_t frameLen = AxpertCRC::appendFramed(frame, sizeof(frame), payloadLen);
    if (frameLen == 0) return false;

    size_t respLen = _transport.transact(frame, frameLen, respBuf, respBufCapacity, resolveTimeout(timeoutMs));
    if (respLen < 2) return false; // need at least the 2 trailing CRC bytes

    if (_verifyCrcOnReceive && !AxpertCRC::verify(respBuf, respLen)) return false;

    outContentLen = respLen - 2; // strip the CRC bytes; leading '(' stays
    return true;
}

template <typename ResponseT>
bool AxpertDevice::transact(const char* payload, size_t payloadLen, ResponseT& out, int32_t timeoutMs) {
    uint8_t respBuf[AXPERT_MAX_FRAME_LEN];
    size_t contentLen = 0;
    if (!transactRaw(payload, payloadLen, respBuf, sizeof(respBuf), contentLen, timeoutMs)) return false;
    return ResponseT::parse(respBuf, contentLen, out);
}

template <typename ResponseT>
bool AxpertDevice::transact(const char* payload, ResponseT& out, int32_t timeoutMs) {
    return transact(payload, strlen(payload), out, timeoutMs);
}

template <typename RequestT>
bool AxpertDevice::transactSet(const RequestT& request, int32_t timeoutMs) {
    char payload[AXPERT_MAX_FRAME_LEN];
    size_t payloadLen = request.build(payload, sizeof(payload));
    if (payloadLen == 0) return false;

    AckResponse ack;
    if (!transact(payload, payloadLen, ack, timeoutMs)) return false;
    return ack.ok;
}

template <typename RequestT>
bool AxpertDevice::transactSet(int32_t timeoutMs) {
    char payload[AXPERT_MAX_FRAME_LEN];
    size_t payloadLen = RequestT::build(payload, sizeof(payload));
    if (payloadLen == 0) return false;

    AckResponse ack;
    if (!transact(payload, payloadLen, ack, timeoutMs)) return false;
    return ack.ok;
}

// ---------------------------------------------------------------------------
// Inquiry commands (protocol section 2)
// ---------------------------------------------------------------------------

bool AxpertDevice::queryProtocolId(ProtocolIdResponse& out, int32_t timeoutMs) {
    return transact("QPI", out, timeoutMs);
}

bool AxpertDevice::querySerialNumber(SerialNumberResponse& out, int32_t timeoutMs) {
    return transact("QID", out, timeoutMs);
}

bool AxpertDevice::querySerialNumberExtended(SerialNumberResponse& out, int32_t timeoutMs) {
    // QSID's reply is parsed by SerialNumberResponse::parseExtended, not
    // the plain ::parse the transact<>() template calls - so this one
    // goes through transactRaw() directly instead.
    uint8_t respBuf[AXPERT_MAX_FRAME_LEN];
    size_t contentLen = 0;
    if (!transactRaw("QSID", 4, respBuf, sizeof(respBuf), contentLen, timeoutMs)) return false;
    return SerialNumberResponse::parseExtended(respBuf, contentLen, out);
}

bool AxpertDevice::queryMainFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs) {
    return transact("QVFW", out, timeoutMs);
}

bool AxpertDevice::querySccFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs) {
    return transact("QVFW2", out, timeoutMs);
}

bool AxpertDevice::queryRemotePanelFirmwareVersion(FirmwareVersionResponse& out, int32_t timeoutMs) {
    return transact("QVFW3", out, timeoutMs);
}

bool AxpertDevice::queryRatingInfo(RatingInfoResponse& out, int32_t timeoutMs) {
    return transact("QPIRI", out, timeoutMs);
}

bool AxpertDevice::queryFlagStatus(FlagStatusResponse& out, int32_t timeoutMs) {
    return transact("QFLAG", out, timeoutMs);
}

bool AxpertDevice::queryGeneralStatus(GeneralStatusResponse& out, int32_t timeoutMs) {
    return transact("QPIGS", out, timeoutMs);
}

bool AxpertDevice::queryDeviceMode(DeviceModeResponse& out, int32_t timeoutMs) {
    return transact("QMOD", out, timeoutMs);
}

bool AxpertDevice::queryWarningStatus(WarningStatusResponse& out, int32_t timeoutMs) {
    return transact("QPIWS", out, timeoutMs);
}

bool AxpertDevice::queryDefaultSettings(DefaultSettingsResponse& out, int32_t timeoutMs) {
    return transact("QDI", out, timeoutMs);
}

bool AxpertDevice::queryMaxChargingCurrentOptions(ChargingCurrentOptionsResponse& out, int32_t timeoutMs) {
    return transact("QMCHGCR", out, timeoutMs);
}

bool AxpertDevice::queryMaxUtilityChargingCurrentOptions(ChargingCurrentOptionsResponse& out, int32_t timeoutMs) {
    return transact("QMUCHGCR", out, timeoutMs);
}

bool AxpertDevice::queryTime(TimeResponse& out, int32_t timeoutMs) {
    // QT is the one command with no CRC at all (protocol 2.19) - build and
    // read the frame directly instead of going through
    // AxpertCRC::appendFramed()/verify().
    uint8_t frame[3] = {'Q', 'T', 0x0D};

    uint8_t respBuf[AXPERT_MAX_FRAME_LEN];
    size_t respLen = _transport.transact(frame, sizeof(frame), respBuf, sizeof(respBuf), resolveTimeout(timeoutMs));
    if (respLen == 0) return false;

    return TimeResponse::parse(respBuf, respLen, out);
}

bool AxpertDevice::queryModelName(ModelNameResponse& out, int32_t timeoutMs) {
    return transact("QMN", out, timeoutMs);
}

bool AxpertDevice::queryGeneralModelName(GeneralModelNameResponse& out, int32_t timeoutMs) {
    return transact("QGMN", out, timeoutMs);
}

bool AxpertDevice::queryBatteryEqualizationStatus(BatteryEqualizationStatusResponse& out, int32_t timeoutMs) {
    return transact("QBEQI", out, timeoutMs);
}

bool AxpertDevice::queryParallelInfo(uint8_t unitIndex, ParallelInfoResponse& out, int32_t timeoutMs) {
    char cmd[8];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QPGS")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, unitIndex, 1)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryTotalPvGeneratedEnergy(EnergyResponse& out, int32_t timeoutMs) {
    return transact("QET", out, timeoutMs);
}

bool AxpertDevice::queryPvGeneratedEnergyOfYear(uint16_t year, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[8];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QEY")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryPvGeneratedEnergyOfMonth(uint16_t year, uint8_t month, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[10];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QEM")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, month, 2)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryPvGeneratedEnergyOfDay(uint16_t year, uint8_t month, uint8_t day, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[12];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QED")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, month, 2)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, day, 2)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryTotalOutputLoadEnergy(EnergyResponse& out, int32_t timeoutMs) {
    return transact("QLT", out, timeoutMs);
}

bool AxpertDevice::queryOutputLoadEnergyOfYear(uint16_t year, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[8];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QLY")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryOutputLoadEnergyOfMonth(uint16_t year, uint8_t month, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[10];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QLM")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, month, 2)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

bool AxpertDevice::queryOutputLoadEnergyOfDay(uint16_t year, uint8_t month, uint8_t day, EnergyResponse& out, int32_t timeoutMs) {
    char cmd[12];
    size_t offset = 0;
    if (!AxpertBuilding::appendText(cmd, sizeof(cmd), &offset, "QLD")) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, year, 4)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, month, 2)) return false;
    if (!AxpertBuilding::appendUInt(cmd, sizeof(cmd), &offset, day, 2)) return false;
    return transact(cmd, offset, out, timeoutMs);
}

// ---------------------------------------------------------------------------
// Setting commands (protocol section 3, plus 2.35 PBMS)
// ---------------------------------------------------------------------------

bool AxpertDevice::startAteTest(int32_t timeoutMs) {
    return transactSet<StartAteTestRequest>(timeoutMs);
}

bool AxpertDevice::endAteTest(int32_t timeoutMs) {
    return transactSet<EndAteTestRequest>(timeoutMs);
}

bool AxpertDevice::setFlags(const SetFlagsRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::resetToDefaults(int32_t timeoutMs) {
    return transactSet<ResetToDefaultsRequest>(timeoutMs);
}

bool AxpertDevice::setMaxChargingCurrent(const SetMaxChargingCurrentRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setMaxUtilityChargingCurrent(const SetMaxUtilityChargingCurrentRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setOutputFrequency(const SetOutputFrequencyRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setOutputRatingVoltage(const SetOutputRatingVoltageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setOutputSourcePriority(const SetOutputSourcePriorityRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setOutputMode(const SetOutputModeRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryVoltageBackToUtility(const SetBatteryVoltageBackToUtilityRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryVoltageBackToBattery(const SetBatteryVoltageBackToBatteryRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setChargerSourcePriority(const SetChargerSourcePriorityRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setParallelChargerSourcePriority(const SetParallelChargerSourcePriorityRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setGridWorkingRange(const SetGridWorkingRangeRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryType(const SetBatteryTypeRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryCutOffVoltage(const SetBatteryCutOffVoltageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryCvChargingVoltage(const SetBatteryCvChargingVoltageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryFloatChargingVoltage(const SetBatteryFloatChargingVoltageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryVoltageAdjustPoint(const SetBatteryVoltageAdjustPointRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::resetBatteryVoltageAdjust(int32_t timeoutMs) {
    return transactSet<ResetBatteryVoltageAdjustRequest>(timeoutMs);
}

bool AxpertDevice::setPvOkCondition(const SetPvOkConditionRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setPvPowerBalance(const SetPvPowerBalanceRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::resetEnergyData(int32_t timeoutMs) {
    return transactSet<ResetEnergyDataRequest>(timeoutMs);
}

bool AxpertDevice::eraseDataLog(int32_t timeoutMs) {
    return transactSet<EraseDataLogRequest>(timeoutMs);
}

bool AxpertDevice::setDataLogPeriod(const SetDataLogPeriodRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationEnabled(const SetBatteryEqualizationEnabledRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationTime(const SetBatteryEqualizationTimeRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationPeriod(const SetBatteryEqualizationPeriodRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationVoltage(const SetBatteryEqualizationVoltageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationOverTime(const SetBatteryEqualizationOverTimeRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBatteryEqualizationActive(const SetBatteryEqualizationActiveRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setMaxChargingTimeAtCv(const SetMaxChargingTimeAtCvRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}

bool AxpertDevice::setBmsMessage(const SetBmsMessageRequest& request, int32_t timeoutMs) {
    return transactSet(request, timeoutMs);
}
