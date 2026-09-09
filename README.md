# AxpertProtocol

An object-oriented Arduino library for the Voltronic/Axpert **MKS II / MKS III / MKS IV** RS232 remote-panel protocol - the same PI30-family protocol used by many Voltronic-based inverters.

- Works with **any RX/TX port**: pass a `HardwareSerial` (any board, any UART), a `SoftwareSerial` (AVR), or any other `Stream`-derived transport - the library doesn't care which.
- Every request and response is a **typed struct**, parsed/built by the library itself - no manual string wrangling.
- **Multiple inverters** at once: each `AxpertDevice` instance is fully independent, no shared/static state.
- A global default timeout per device, with a **per-call override** for the handful of commands that need more time.
- CRC verification on received replies can be turned off per device for debugging, without weakening what gets *sent*.

> **Status:** every command still present in the source protocol document has a request/response struct and a wired-up `AxpertDevice` method, and the whole thing compiles clean (`--warnings all`) against both AVR and ESP32. **Confirmed working against a real inverter** - not every single command has been exercised there, and a command that comes back unsupported on a given device is a limitation of that inverter/firmware, not of this library (see [Checking whether a command is supported](#checking-whether-a-command-is-supported)). See [Known issues](#known-issues) below for the handful of spots where the document itself is ambiguous or was found to be inconsistent (tracked-changes marks that didn't get cleaned up).

## Installation

No package-manager listing yet - install from source:

- **Arduino IDE:** copy (or clone) this folder into your sketchbook's `libraries/` directory (usually `Documents/Arduino/libraries/AxpertProtocol`), then restart the IDE.
- **PlatformIO:** copy this folder into your project's `lib/` directory, or point `lib_deps` at wherever you've cloned it.

## Quick start

```cpp
#include <AxpertProtocol.h>

// Any HardwareSerial works on any board. Serial1 here keeps `Serial` free
// for USB debug output.
AxpertDevice inverter(Serial1);

void setup() {
    Serial.begin(115200);
    inverter.begin(); // configures Serial1 at the protocol's 2400 8N1 for you
}

void loop() {
    GeneralStatusResponse status;
    if (inverter.queryGeneralStatus(status)) {
        Serial.print("Battery voltage: ");
        Serial.println(status.batteryVoltage);
    }
    delay(1000);
}
```

See `examples/` for more: `MultipleInverters` (two trackers at once), `CustomSerialPort` (arbitrary pins on AVR via SoftwareSerial), `ESP32PinRemap` (arbitrary pins on ESP32 via hardware UART remapping), `RawConsole` (type any bare command into the Serial Monitor, forwarded via `sendRawCommand()` - handy for probing a command this library doesn't have a typed method for yet).

### Choosing a constructor

| You have... | Use | Notes |
|---|---|---|
| A `HardwareSerial` (`Serial1`, `Serial2`, ...) | `AxpertDevice(port)` | Works on every architecture. `begin()` configures it for you. |
| Nothing free but you're on AVR | `AxpertDevice(rxPin, txPin)` | Owns an internal `SoftwareSerial`; `begin()` configures it. |
| A `SoftwareSerial` you already made | `AxpertDevice(port)` | Same overload as above, just passed a `SoftwareSerial&` - `begin()` still configures it. |
| Nothing free but you're on ESP32 | `AxpertDevice(port, rxPin, txPin)` | Remaps a real hardware UART instead of bit-banging - `port` is typically `Serial1`/`Serial2`. |
| Anything else (RS485 wrapper, mock transport, ...) | `AxpertDevice(port)` (generic `Stream&`) | You must call the port's own `begin()`/equivalent yourself first - `begin()` is a no-op here. |

**Always call `AxpertDevice::begin()` from `setup()`, never at global/namespace scope.** Touching hardware before the Arduino runtime has finished initializing it is undefined on several platforms - that's why the constructors only *record* the port, and `begin()` (called explicitly, later, from `setup()`) does the actual configuration.

## Timeouts

Every `AxpertDevice` has one default timeout, set in the constructor (`AXPERT_DEFAULT_TIMEOUT_MS` = 1000 ms if you don't pass one) and changeable later:

```cpp
AxpertDevice inverter(Serial1, 800);   // this device's default is now 800 ms
inverter.setDefaultTimeout(300);       // ...or change it any time
uint16_t current = inverter.defaultTimeout();
```

Every single `query*()`/`set*()` method also takes its own `timeoutMs` as the *last* parameter, defaulting to `-1`:

```cpp
bool queryGeneralStatus(GeneralStatusResponse& out, int32_t timeoutMs = -1);
```

`-1` means "use this device's default timeout" - that's the normal case, so you never have to pass it. Pass a non-negative value to override the timeout for *that one call only*, without touching the device's default for every other call:

```cpp
GeneralStatusResponse status;
inverter.queryGeneralStatus(status);            // uses the device's default timeout
inverter.queryGeneralStatus(status, 1500);      // this call only: wait up to 1500 ms
```

This matters because a few commands (e.g. `queryParallelInfo`, the energy queries) can take noticeably longer to answer than a plain `QPIGS`, and you don't want to pay for that longer wait on every other call just because one command needs it.

## CRC verification

CRC verification on *received* replies is on by default and controlled per device:

```cpp
inverter.setVerifyCrcOnReceive(false); // stop rejecting replies with a bad/missing CRC
bool checking = inverter.verifyCrcOnReceive();
```

This only affects how incoming responses are checked - every request this library sends is always framed with a correct CRC regardless of this setting. It exists for debugging against a device (or a captured log) with nonconformant CRC framing; leave it enabled for normal use, since it's your only defense against acting on a corrupted reply.

## Sending raw commands

For anything this library doesn't have a typed method for yet - or to probe/replay a command verbatim - `sendRawCommand()` is the escape hatch every `query*()`/`set*()` method is itself built on:

```cpp
char response[64];
if (inverter.sendRawCommand("QPIGS", response, sizeof(response))) {
    Serial.println(response); // e.g. "(228.7 50.0 230.0 ..."
}
```

`command` is a bare command string - no CRC, no trailing `<cr>`, just like `"QPIGS"` above. On success, `response` holds the reply as a null-terminated string (leading `(` included, any trailing CRC bytes stripped), so `responseBufCapacity` needs room for that terminator.

`appendCRC` (the 4th parameter, default `true`) controls only the *outgoing* side:

```cpp
inverter.sendRawCommand("QPIGS", response, sizeof(response));        // CRC + <cr> appended, reply's CRC checked as usual
inverter.sendRawCommand("QT", response, sizeof(response), false);    // bare <cr> only, no CRC on either side
```

Pass `false` for a command you know is genuinely CRC-less, the way `QT` (2.19) is - the same shape `queryTime()` uses internally. `timeoutMs` works exactly like every other method's (last parameter, `-1` = this device's default).

## Checking whether a command is supported

Not every Axpert-protocol device implements every inquiry command. Every `query*()` method has a matching `isXxxSupported()` (e.g. `queryGeneralStatus()` -> `isGeneralStatusSupported()`) that sends the same query up to 3 times and returns `true` on the first successful reply, `false` if all 3 time out or come back malformed - a single dropped byte on the wire shouldn't read as "not supported":

```cpp
if (inverter.isBatteryEqualizationStatusSupported()) {
    BatteryEqualizationStatusResponse status;
    inverter.queryBatteryEqualizationStatus(status);
}
```

Methods that take extra parameters keep them on the `isXxxSupported()` side too, e.g. `isParallelInfoSupported(unitIndex)` or `isPvGeneratedEnergyOfMonthSupported(year, month)`; `timeoutMs` is still the last parameter (`-1` = this device's default, applied to each of the up-to-3 attempts individually). There's no `isXxxSupported()` for `set*()` methods - the protocol has no read-only way to probe a setting command without actually changing something.

## Architecture

```
src/
  AxpertDevice.h/.cpp     the facade: one instance per physical inverter
  AxpertTransport.h/.cpp  blocking send/receive over a Stream
  AxpertCRC.h/.cpp        CRC-16/XMODEM + the protocol's byte-escaping quirk
  AxpertParsing.h/.cpp    shared tokenizer/number-parsing helpers for responses
  AxpertBuilding.h/.cpp   shared string-building helpers for requests
  AxpertTypes.h           every enum the protocol defines (modes, priorities, fault codes, ...)
  responses/              one struct + parse() per inquiry command
  requests/                one struct + build() per setting command
```

Every `AxpertDevice::query*()` method fills a typed `...Response` struct; every `set*()` method takes a typed `...Request` struct. Internally, both go through the same generic transaction helper (frame with CRC, send, verify the reply's CRC, hand off to the matching struct) - see the comment block above `AxpertDevice::transactRaw()` for the full flow.

## Supported commands

### Inquiry commands (protocol section 2)

| # | Command | Method | Response struct |
|---|---|---|---|
| 2.1 | QPI | `queryProtocolId` | `ProtocolIdResponse` |
| 2.2 | QID | `querySerialNumber` | `SerialNumberResponse` |
| 2.3 | QSID | `querySerialNumberExtended` | `SerialNumberResponse` |
| 2.4 | QVFW | `queryMainFirmwareVersion` | `FirmwareVersionResponse` |
| 2.5 | QVFW2 | `querySccFirmwareVersion` | `FirmwareVersionResponse` |
| 2.6 | QVFW3 | `queryRemotePanelFirmwareVersion` | `FirmwareVersionResponse` |
| 2.8 | QPIRI | `queryRatingInfo` | `RatingInfoResponse` |
| 2.9 | QFLAG | `queryFlagStatus` | `FlagStatusResponse` |
| 2.10 | QPIGS | `queryGeneralStatus` | `GeneralStatusResponse` |
| 2.11 | QMOD | `queryDeviceMode` | `DeviceModeResponse` |
| 2.12 | QPIWS | `queryWarningStatus` | `WarningStatusResponse` |
| 2.13 | QDI | `queryDefaultSettings` | `DefaultSettingsResponse` |
| 2.14 | QMCHGCR | `queryMaxChargingCurrentOptions` | `ChargingCurrentOptionsResponse` |
| 2.15 | QMUCHGCR | `queryMaxUtilityChargingCurrentOptions` | `ChargingCurrentOptionsResponse` |
| 2.19 | QT | `queryTime` | `TimeResponse` (no CRC - protocol's own exception) |
| 2.20 | QMN | `queryModelName` | `ModelNameResponse` |
| 2.21 | QGMN | `queryGeneralModelName` | `GeneralModelNameResponse` |
| 2.22 | QBEQI | `queryBatteryEqualizationStatus` | `BatteryEqualizationStatusResponse` |
| 2.23 | QPGSn | `queryParallelInfo(unitIndex, ...)` | `ParallelInfoResponse` |
| 2.25 | QET | `queryTotalPvGeneratedEnergy` | `EnergyResponse` |
| 2.26 | QEY | `queryPvGeneratedEnergyOfYear(year, ...)` | `EnergyResponse` |
| 2.27 | QEM | `queryPvGeneratedEnergyOfMonth(year, month, ...)` | `EnergyResponse` |
| 2.28 | QED | `queryPvGeneratedEnergyOfDay(year, month, day, ...)` | `EnergyResponse` |
| 2.29 | QLT | `queryTotalOutputLoadEnergy` | `EnergyResponse` |
| 2.30 | QLY | `queryOutputLoadEnergyOfYear(year, ...)` | `EnergyResponse` |
| 2.31 | QLM | `queryOutputLoadEnergyOfMonth(year, month, ...)` | `EnergyResponse` |
| 2.32 | QLD | `queryOutputLoadEnergyOfDay(year, month, day, ...)` | `EnergyResponse` |
| 2.34 | QBMS | *not implemented* | undocumented in the source PDF (no field table at all); see [Known issues](#known-issues) |

### Setting commands (protocol section 3, plus 2.35)

| # | Command | Method | Request struct |
|---|---|---|---|
| 3.1 | ATE1 | `startAteTest` | `StartAteTestRequest` |
| 3.2 | ATE0 | `endAteTest` | `EndAteTestRequest` |
| 3.3 | PE/PD | `setFlags` | `SetFlagsRequest` |
| 3.4 | PF | `resetToDefaults` | `ResetToDefaultsRequest` |
| 3.5 | MNCHGC | `setMaxChargingCurrent` | `SetMaxChargingCurrentRequest` |
| 3.6 | MUCHGC | `setMaxUtilityChargingCurrent` | `SetMaxUtilityChargingCurrentRequest` |
| 3.7 | F | `setOutputFrequency` | `SetOutputFrequencyRequest` |
| 3.8 | POPV | `setOutputRatingVoltage` | `SetOutputRatingVoltageRequest` |
| 3.9 | POP | `setOutputSourcePriority` | `SetOutputSourcePriorityRequest` |
| 3.10 | POPM | `setOutputMode` | `SetOutputModeRequest` |
| 3.12 | PBCV | `setBatteryVoltageBackToUtility` | `SetBatteryVoltageBackToUtilityRequest` |
| 3.13 | PBDV | `setBatteryVoltageBackToBattery` | `SetBatteryVoltageBackToBatteryRequest` |
| 3.14 | PCP | `setChargerSourcePriority` | `SetChargerSourcePriorityRequest` |
| 3.15 | PPCP | `setParallelChargerSourcePriority` | `SetParallelChargerSourcePriorityRequest` |
| 3.16 | PGR | `setGridWorkingRange` | `SetGridWorkingRangeRequest` |
| 3.17 | PBT | `setBatteryType` | `SetBatteryTypeRequest` |
| 3.18 | PSDV | `setBatteryCutOffVoltage` | `SetBatteryCutOffVoltageRequest` |
| 3.19 | PCVV | `setBatteryCvChargingVoltage` | `SetBatteryCvChargingVoltageRequest` |
| 3.20 | PBFT | `setBatteryFloatChargingVoltage` | `SetBatteryFloatChargingVoltageRequest` |
| 3.21/3.22 | BTA1/BTA2 | `setBatteryVoltageAdjustPoint` | `SetBatteryVoltageAdjustPointRequest` (shared, `point` field picks 1 or 2) |
| 3.23 | BTA0 | `resetBatteryVoltageAdjust` | `ResetBatteryVoltageAdjustRequest` |
| 3.24 | PPVOKC | `setPvOkCondition` | `SetPvOkConditionRequest` |
| 3.25 | PSPB | `setPvPowerBalance` | `SetPvPowerBalanceRequest` |
| 3.26 | RTEY | `resetEnergyData` | `ResetEnergyDataRequest` |
| 3.27 | RTDL | `eraseDataLog` | `EraseDataLogRequest` |
| 3.28 | PDL | `setDataLogPeriod` | `SetDataLogPeriodRequest` |
| 3.29 | PBEQE | `setBatteryEqualizationEnabled` | `SetBatteryEqualizationEnabledRequest` |
| 3.30 | PBEQT | `setBatteryEqualizationTime` | `SetBatteryEqualizationTimeRequest` |
| 3.31 | PBEQP | `setBatteryEqualizationPeriod` | `SetBatteryEqualizationPeriodRequest` |
| 3.32 | PBEQV | `setBatteryEqualizationVoltage` | `SetBatteryEqualizationVoltageRequest` |
| 3.33 | PBEQOT | `setBatteryEqualizationOverTime` | `SetBatteryEqualizationOverTimeRequest` |
| 3.34 | PBEQA | `setBatteryEqualizationActive` | `SetBatteryEqualizationActiveRequest` |
| 3.35 | PCVT | `setMaxChargingTimeAtCv` | `SetMaxChargingTimeAtCvRequest` |
| 2.35 | PBMS | `setBmsMessage` | `SetBmsMessageRequest` (reply is a plain `AckResponse`, like every setter above) |

### Removed in this protocol revision (not implemented, on purpose)

The source PDF turned out to be a redlined/edited document - plain text extraction silently merged struck-through (deleted) text with the surviving text, which was only caught by re-extracting with strikethrough/color detection. These commands are struck through in the source and have no struct or method:

VERFW (2.7), QOPPT/QCHPT/QOPCHT (2.16-2.18), Q2PGSn (2.24), QBATCD (2.33), QLED (2.36), POPLG (3.11), SRS (3.36), DAT (3.37), PBATMAXDISC (3.38), PBATCD (3.39), PLEDE/PLEDS/PLEDM/PLEDB/PLEDT/PLEDC (3.40-3.45).

## Known issues

A few spots where the protocol document itself is unclear, even after accounting for the redlining above - flagged in code comments where relevant, listed here for visibility:

- **`AxpertWarningBit::MksiiiFaultCodeBit0/1`** (QPIWS bits a32/a33): the document's own revision note is internally inconsistent about which bits encode this on which model. Verify against real hardware before relying on it.
- **`EndAteTestRequest` (ATE0)**: the document's section title omits `<CRC><cr>`, unlike ATE1's - it may need to be sent without CRC framing. Not yet verified.
- **QBMS (2.34)**: no field-level documentation exists in the source PDF at all, and no public reference for the format was found either - deliberately left unimplemented rather than guessing.

## Protocol reference

This library is based on `Axpert MKS II&MKS III&MKS IV RS232 Protocol 20201109.pdf`, included in this repository. The PDF turned out to be a redlined/edited document (see [Removed in this protocol revision](#removed-in-this-protocol-revision-not-implemented-on-purpose) above) - if you're checking a field against it yourself, make sure your PDF viewer isn't hiding strikethrough/tracked-changes formatting, or you'll see commands and fields that no longer apply.

## License

LGPL-3.0-or-later - see [LICENSE](LICENSE) (and [COPYING](COPYING), the GNU GPL that the LGPL incorporates by reference). In short: you're free to use this library as-is in any project, open or closed source; but if you modify the *library's own files* and distribute that modified version (e.g. as part of a product), you have to make your changes to those files available under the same license. Contributions via pull request are very welcome.
