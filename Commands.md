# Command Reference

A field-by-field reference for every struct in this library, generated from the actual `src/responses/` and `src/requests/` headers - not a re-transcription of the protocol PDF. Commands the source document struck through (see [README - Removed in this protocol revision](README.md#removed-in-this-protocol-revision-not-implemented-on-purpose)) are left out entirely, as are the "reserved" wire fields that get consumed while parsing but never exposed on a struct.

**How to read the tables:** *Range / Values* is derived from the wire field's digit width (e.g. a 3-digit field `nnn` → `0-999`) unless the protocol states a narrower constraint (e.g. "50 or 60"), in which case that's shown instead. Where the field's C++ type has less range than the wire field allows for, the *Notes* column says so - the device just doesn't realistically send values that large.

## Enums

Referenced by the `Type` column below; every enum lives in `src/AxpertTypes.h`.

| Enum | Values |
|---|---|
| `AxpertDeviceMode` | `PowerOn`, `Standby`, `Line`, `Battery`, `Fault`, `Shutdown`, `Charge`, `Bypass`, `Eco`, `Unknown` |
| `AxpertOutputSourcePriority` | `UtilityFirst`=0, `SolarFirst`=1, `SbuFirst`=2 |
| `AxpertChargerSourcePriority` | `UtilityFirst`=0, `SolarFirst`=1, `SolarAndUtility`=2, `SolarOnly`=3 |
| `AxpertBatteryType` | `Agm`=0, `Flooded`=1, `User`=2, `Pylontech`=3, `Shinheung`=4, `Weco`=5, `Soltaro`=6 |
| `AxpertInputVoltageRange` | `Appliance`=0, `Ups`=1 |
| `AxpertOutputMode` | `SingleMachine`=0, `Parallel`=1, `Phase1Of3`=2, `Phase2Of3`=3, `Phase3Of3`=4 |
| `AxpertMachineType` | `GridTie`=0, `OffGrid`=1, `Hybrid`=10 (not 2 - wire value is the literal 2-digit code) |
| `AxpertTopology` | `Transformerless`=0, `Transformer`=1 |
| `AxpertPvOkConditionForParallel` | `AnyUnitConnected`=0, `AllUnitsConnected`=1 |
| `AxpertPvPowerBalance` | `MaxCurrentIsMaxChargedCurrent`=0, `MaxPowerIsChargedPlusLoadPower`=1 |
| `AxpertFlagState` | `Disabled`=-1, `Unchanged`=0, `Enabled`=1 |
| `AxpertChargingSourceStatus` | `None`=0b000, `SccOnly`=0b110, `AcOnly`=0b101, `SccAndAc`=0b111 (only these 4 patterns are valid) |
| `AxpertParallelBatteryStatus` | `Normal`=0, `Under`=1, `Open`=2 |
| `AxpertFaultCode` | `None`=0, `FanLocked`=1, `OverTemperature`=2, `BatteryVoltageTooHigh`=3, `BatteryVoltageTooLow`=4, `OutputShortCircuitedOrOverTemperature`=5, `OutputVoltageTooHigh`=6, `OverLoadTimeOut`=7, `BusVoltageTooHigh`=8, `BusSoftStartFailed`=9, `PvChargerCurrentOver`=10, `PvOverVolt`=11, `DcDcOverCurrent`=12, `OverCurrentInverter`=51, `InverterSoftStartFailed`=53, `OverDcVoltageOnOutputOfInverter`=55, `CurrentSensorFailed`=57, `OutputVoltageTooLow`=58, `PvVoltageHigh`=59, `InverterNegativePower`=60, `ParallelVersionDifferent`=71, `OutputCircuitFailed`=72, `OutputVoltDifferent`=73, `CanCommunicationFailed`=80, `ParallelHostLineLost`=81, `ParallelSynchronizedSignalLost`=82, `ParallelBatteryVoltageDetectDifferent`=83, `ParallelLineVoltageOrFrequencyDetectDifferent`=84, `ParallelOutputSettingDifferent`=86 |
| `AxpertWarningBit` | Bit *index* (0-35) into `WarningStatusResponse`, not a value stored anywhere - see that struct's table below for the field names. |

## Inquiry commands (responses)

### 2.1 QPI - `ProtocolIdResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `protocolId` | `uint8_t` | 0-99 | 30 for the Axpert series |

### 2.2 / 2.3 QID / QSID - `SerialNumberResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `serial` | `char[21]` | up to 20 ASCII chars, null-terminated | `AXPERT_SERIAL_MAX_LEN` = 20 |
| `length` | `uint8_t` | 0-20 | always 14 when filled by `parse()` (QID); the actual reported length when filled by `parseExtended()` (QSID) |

### 2.4 / 2.5 / 2.6 QVFW / QVFW2 / QVFW3 - `FirmwareVersionResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `seriesNumber` | `uint32_t` | 0-0xFFFFF (0-1048575) | 5 hex digits on the wire |
| `version` | `uint8_t` | 0-0xFF (0-255) | 2 hex digits on the wire |

### 2.8 QPIRI - `RatingInfoResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `acOutputRatingVoltage` | `float` | 0.0-999.9 (V) | |
| `acOutputRatingCurrent` | `float` | 0.0-99.9 (A) | |
| `acOutputRatingVoltage2` | `float` | 0.0-999.9 (V) | protocol repeats the AC output voltage field a second time |
| `acOutputRatingFrequency` | `float` | 0.0-99.9 (Hz) | |
| `acOutputRatingCurrent2` | `float` | 0.0-99.9 (A) | |
| `acOutputRatingApparentPower` | `uint16_t` | 0-9999 (VA) | |
| `acOutputRatingActivePower` | `uint16_t` | 0-9999 (W) | |
| `batteryRatingVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryRechargeVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryUnderVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryBulkVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryFloatVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryType` | `AxpertBatteryType` | see [Enums](#enums) | |
| `maxAcChargingCurrent` | `uint16_t` | 0-999 (A) | |
| `maxChargingCurrent` | `uint16_t` | 0-999 (A) | |
| `inputVoltageRange` | `AxpertInputVoltageRange` | see [Enums](#enums) | |
| `outputSourcePriority` | `AxpertOutputSourcePriority` | see [Enums](#enums) | |
| `chargerSourcePriority` | `AxpertChargerSourcePriority` | see [Enums](#enums) | |
| `parallelMaxNumber` | `uint8_t` | 0-9 | |
| `machineType` | `AxpertMachineType` | see [Enums](#enums) | |
| `topology` | `AxpertTopology` | see [Enums](#enums) | |
| `outputMode` | `AxpertOutputMode` | see [Enums](#enums) | |
| `batteryRedischargeVoltage` | `float` | 0.0-99.9 (V) | |
| `pvOkConditionForParallel` | `AxpertPvOkConditionForParallel` | see [Enums](#enums) | |
| `pvPowerBalance` | `AxpertPvPowerBalance` | see [Enums](#enums) | |
| `maxChargingTimeAtCvMinutes` | `uint16_t` | 0-999 (minutes) | |

### 2.9 QFLAG - `FlagStatusResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `silenceBuzzerEnabled` | `bool` | true/false | |
| `overloadBypassEnabled` | `bool` | true/false | |
| `lcdEscapeToDefaultEnabled` | `bool` | true/false | |
| `overloadRestartEnabled` | `bool` | true/false | |
| `overTemperatureRestartEnabled` | `bool` | true/false | |
| `backlightOnEnabled` | `bool` | true/false | |
| `alarmOnPrimarySourceInterruptEnabled` | `bool` | true/false | |
| `faultCodeRecordEnabled` | `bool` | true/false | |

### 2.10 QPIGS - `GeneralStatusResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `gridVoltage` | `float` | 0.0-999.9 (V) | |
| `gridFrequency` | `float` | 0.0-99.9 (Hz) | |
| `acOutputVoltage` | `float` | 0.0-999.9 (V) | |
| `acOutputFrequency` | `float` | 0.0-99.9 (Hz) | |
| `acOutputApparentPower` | `uint16_t` | 0-9999 (VA) | |
| `acOutputActivePower` | `uint16_t` | 0-9999 (W) | |
| `outputLoadPercent` | `uint8_t` | 0-100 typical (%) | wire field is 3 digits (0-999); stored as `uint8_t` (max 255) |
| `busVoltage` | `uint16_t` | 0-999 (V) | |
| `batteryVoltage` | `float` | 0.00-99.99 (V) | |
| `batteryChargingCurrent` | `uint16_t` | 0-999 (A) | |
| `batteryCapacityPercent` | `uint8_t` | 0-100 typical (%) | wire field is 3 digits (0-999); stored as `uint8_t` (max 255) |
| `inverterHeatSinkTemperature` | `uint16_t` | 0-9999 (°C) | |
| `pvInputCurrent` | `float` | 0.0-99.9 (A) | |
| `pvInputVoltage` | `float` | 0.0-999.9 (V) | |
| `batteryVoltageFromScc` | `float` | 0.00-99.99 (V) | |
| `batteryDischargeCurrent` | `uint16_t` | 0-9999 typical (A) | wire field is 5 digits (0-99999); stored as `uint16_t` (max 65535) |
| `isPVFeedingLoad` | `uint8_t` | 0 or 1 | b7: 1 = PV or AC feeding the load |
| `isConfigurationChanged` | `uint8_t` | 0 or 1 | b6: 1 = configuration changed |
| `isSccFirmwareUpdated` | `uint8_t` | 0 or 1 | b5: 1 = SCC firmware updated |
| `isLoadOn` | `uint8_t` | 0 or 1 | b4 |
| `isCharging` | `uint8_t` | 0 or 1 | b2: overall charging on/off (b3 is reserved and skipped) |
| `isSCCCharging` | `uint8_t` | 0 or 1 | b1 |
| `isACCharging` | `uint8_t` | 0 or 1 | b0 |
| `batteryVoltageOffsetForFans10mV` | `uint8_t` | 0-99 (x10 mV) | |
| `eepromVersion` | `uint8_t` | 0-99 | |
| `pvChargingPower` | `uint32_t` | 0-99999 (W) | |
| `isFloatingMode` | `uint8_t` | 0 or 1 | b10: charging-to-floating-mode flag |
| `isSwitchOn` | `uint8_t` | 0 or 1 | b9 |
| `isDustproofInstalled` | `uint8_t` | 0 or 1 | b8 |

### 2.11 QMOD - `DeviceModeResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `mode` | `AxpertDeviceMode` | see [Enums](#enums) | `Unknown` if the device sends a letter this library doesn't recognize |

### 2.12 QPIWS - `WarningStatusResponse`

One `uint8_t` (0 or 1) per bit, named after `AxpertWarningBit`. See that enum's comment in `AxpertTypes.h` for which bits are reserved/ambiguous in this protocol revision. `hasAnyWarning() const` (a plain `bool`-returning method, not a wire field) is true if any of the 36 bits below is set - it doesn't try to distinguish fault- from warning-severity, since the protocol itself is inconsistent about that for several bits.

| Field | Type | Range / Values | Bit |
|---|---|---|---|
| `pvLoss` | `uint8_t` | 0 or 1 | a0 |
| `inverterFault` | `uint8_t` | 0 or 1 | a1 |
| `busOver` | `uint8_t` | 0 or 1 | a2 |
| `busUnder` | `uint8_t` | 0 or 1 | a3 |
| `busSoftFail` | `uint8_t` | 0 or 1 | a4 |
| `lineFail` | `uint8_t` | 0 or 1 | a5 |
| `outputShort` | `uint8_t` | 0 or 1 | a6 |
| `inverterVoltageTooLow` | `uint8_t` | 0 or 1 | a7 |
| `inverterVoltageTooHigh` | `uint8_t` | 0 or 1 | a8 |
| `overTemperature` | `uint8_t` | 0 or 1 | a9 |
| `fanLocked` | `uint8_t` | 0 or 1 | a10 |
| `batteryVoltageHigh` | `uint8_t` | 0 or 1 | a11 |
| `batteryLowAlarm` | `uint8_t` | 0 or 1 | a12 |
| `reserved13` | `uint8_t` | 0 or 1 | a13 |
| `batteryUnderShutdown` | `uint8_t` | 0 or 1 | a14 |
| `batteryDerating` | `uint8_t` | 0 or 1 | a15 |
| `overLoad` | `uint8_t` | 0 or 1 | a16 |
| `eepromFault` | `uint8_t` | 0 or 1 | a17 |
| `inverterOverCurrent` | `uint8_t` | 0 or 1 | a18 |
| `inverterSoftFail` | `uint8_t` | 0 or 1 | a19 |
| `selfTestFail` | `uint8_t` | 0 or 1 | a20 |
| `outputDcVoltageOver` | `uint8_t` | 0 or 1 | a21 |
| `batteryOpen` | `uint8_t` | 0 or 1 | a22 |
| `currentSensorFail` | `uint8_t` | 0 or 1 | a23 |
| `reserved24` | `uint8_t` | 0 or 1 | a24 |
| `reserved25` | `uint8_t` | 0 or 1 | a25 |
| `pvVoltageHigh` | `uint8_t` | 0 or 1 | a26 |
| `pvOverCurrent` | `uint8_t` | 0 or 1 | a27 |
| `reserved28` | `uint8_t` | 0 or 1 | a28 |
| `reserved29` | `uint8_t` | 0 or 1 | a29 |
| `dcDcOverCurrent` | `uint8_t` | 0 or 1 | a30 |
| `reserved31` | `uint8_t` | 0 or 1 | a31 |
| `mksiiiFaultCodeBit0` | `uint8_t` | 0 or 1 | a32 |
| `mksiiiFaultCodeBit1` | `uint8_t` | 0 or 1 | a33 |
| `reserved34` | `uint8_t` | 0 or 1 | a34 |
| `reserved35` | `uint8_t` | 0 or 1 | a35 |

### 2.13 QDI - `DefaultSettingsResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `acOutputVoltage` | `float` | 0.0-999.9 (V) | |
| `acOutputFrequency` | `float` | 0.0-99.9 (Hz) | |
| `maxAcChargingCurrent` | `uint16_t` | 0-99 (A) | wire field is `00DD` - fixed `00` prefix, only the last 2 digits vary |
| `batteryUnderVoltage` | `float` | 0.0-99.9 (V) | |
| `chargingFloatVoltage` | `float` | 0.0-99.9 (V) | |
| `chargingBulkVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryRechargeVoltage` | `float` | 0.0-99.9 (V) | |
| `maxChargingCurrent` | `uint16_t` | 0-99 (A) | |
| `inputVoltageRange` | `AxpertInputVoltageRange` | see [Enums](#enums) | |
| `outputSourcePriority` | `AxpertOutputSourcePriority` | see [Enums](#enums) | |
| `chargerSourcePriority` | `AxpertChargerSourcePriority` | see [Enums](#enums) | |
| `batteryType` | `AxpertBatteryType` | see [Enums](#enums) | |
| `buzzerEnabled` | `bool` | true/false | |
| `powerSavingEnabled` | `bool` | true/false | |
| `overloadRestartEnabled` | `bool` | true/false | |
| `overTemperatureRestartEnabled` | `bool` | true/false | |
| `lcdBacklightEnabled` | `bool` | true/false | |
| `alarmOnPrimarySourceInterruptEnabled` | `bool` | true/false | |
| `faultCodeRecordEnabled` | `bool` | true/false | |
| `overloadBypassEnabled` | `bool` | true/false | |
| `lcdEscapeToDefaultEnabled` | `bool` | true/false | |
| `outputMode` | `AxpertOutputMode` | see [Enums](#enums) | |
| `batteryRedischargeVoltage` | `float` | 0.0-99.9 (V) | |
| `pvOkConditionForParallel` | `AxpertPvOkConditionForParallel` | see [Enums](#enums) | |
| `pvPowerBalance` | `AxpertPvPowerBalance` | see [Enums](#enums) | |
| `maxChargingTimeAtCvMinutes` | `uint16_t` | 0-999 (minutes) | |

### 2.14 / 2.15 QMCHGCR / QMUCHGCR - `ChargingCurrentOptionsResponse`

Same struct for both commands - only the query differs.

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `values` | `uint16_t[16]` | each entry 0-999 (A) | `AXPERT_MAX_CHARGING_CURRENT_OPTIONS` = 16; protocol allows an unbounded list, this is a practical cap |
| `count` | `uint8_t` | 0-16 | how many entries in `values` are actually filled |

### 2.19 QT - `TimeResponse`

No CRC on this command at all (protocol's own exception).

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `year` | `uint16_t` | 0-9999 | |
| `month` | `uint8_t` | 1-12 | |
| `day` | `uint8_t` | 1-31 | |
| `hour` | `uint8_t` | 0-23 | |
| `minute` | `uint8_t` | 0-59 | |
| `second` | `uint8_t` | 0-59 | |

### 2.20 QMN - `ModelNameResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `name` | `char[17]` | up to 16 ASCII chars, null-terminated | `AXPERT_MODEL_NAME_MAX_LEN` = 16 |
| `ratedOutputVa` | `uint16_t` | 0-9999 (VA) | |

### 2.21 QGMN - `GeneralModelNameResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `modelCode` | `uint16_t` | 0-999 | numeric model code, see protocol 2.21's table for the name mapping (e.g. 022 = "AXPERT MKS II 5KW") |

### 2.22 QBEQI - `BatteryEqualizationStatusResponse`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `enabled` | `bool` | true/false | |
| `equalizationTimeMinutes` | `uint16_t` | 0-999 (minutes) | |
| `equalizationPeriodDays` | `uint16_t` | 0-999 (days) | |
| `equalizationMaxCurrent` | `uint16_t` | 0-999 (A) | |
| `equalizationVoltage` | `float` | 0.00-99.99 (V) | |
| `equalizationOverTimeMinutes` | `uint16_t` | 0-999 (minutes) | |
| `active` | `bool` | true/false | |
| `equalizationElapseTimeHours` | `uint32_t` | 0-9999 (hours) | |

*(Two wire fields, `FFF` and `HHH`, are documented only as "reserved" and are intentionally not exposed on this struct.)*

### 2.23 QPGSn - `ParallelInfoResponse`

`n` (which parallel unit to query) is a parameter to `AxpertDevice::queryParallelInfo(unitIndex, ...)`, not part of the response.

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `exists` | `bool` | true/false | whether this parallel unit exists |
| `serialNumber` | `char[15]` | 14 ASCII chars, null-terminated | |
| `workMode` | `AxpertDeviceMode` | see [Enums](#enums) | |
| `faultCode` | `AxpertFaultCode` | see [Enums](#enums) | `None` when there's no fault |
| `gridVoltage` | `float` | 0.0-999.9 (V) | |
| `gridFrequency` | `float` | 0.00-99.99 (Hz) | |
| `acOutputVoltage` | `float` | 0.0-999.9 (V) | |
| `acOutputFrequency` | `float` | 0.00-99.99 (Hz) | |
| `acOutputApparentPower` | `uint16_t` | 0-9999 (VA) | |
| `acOutputActivePower` | `uint16_t` | 0-9999 (W) | |
| `loadPercent` | `uint8_t` | 0-100 typical (%) | wire field is 3 digits (0-999); stored as `uint8_t` (max 255) |
| `batteryVoltage` | `float` | 0.0-99.9 (V) | |
| `batteryChargingCurrent` | `uint16_t` | 0-999 (A) | |
| `batteryCapacityPercent` | `uint8_t` | 0-100 typical (%) | wire field is 3 digits (0-999); stored as `uint8_t` (max 255) |
| `pvInputVoltage` | `float` | 0.0-999.9 (V) | |
| `totalChargingCurrent` | `uint16_t` | 0-999 (A) | |
| `totalAcOutputApparentPower` | `uint32_t` | 0-99999 (VA) | |
| `totalOutputActivePower` | `uint32_t` | 0-99999 (W) | |
| `totalAcOutputPercent` | `uint8_t` | 0-100 typical (%) | wire field is 3 digits (0-999); stored as `uint8_t` (max 255) |
| `isSccOk` | `uint8_t` | 0 or 1 | b7: 1 = SCC OK, 0 = SCC loss |
| `isAcCharging` | `uint8_t` | 0 or 1 | b6 |
| `isSccCharging` | `uint8_t` | 0 or 1 | b5 |
| `batteryStatus` | `AxpertParallelBatteryStatus` | see [Enums](#enums) | b4b3 |
| `isLineLoss` | `uint8_t` | 0 or 1 | b2 |
| `isLoadOn` | `uint8_t` | 0 or 1 | b1 |
| `isConfigurationChanged` | `uint8_t` | 0 or 1 | b0 |
| `outputMode` | `AxpertOutputMode` | see [Enums](#enums) | |
| `chargerSourcePriority` | `AxpertChargerSourcePriority` | see [Enums](#enums) | |
| `maxChargerCurrent` | `uint16_t` | 0-999 (A) | |
| `maxChargerRange` | `uint16_t` | 0-999 (A) | |
| `maxAcChargerCurrent` | `uint16_t` | 0-999 (A) | |
| `pvInputCurrent` | `uint8_t` | 0-99 (A) | |
| `batteryDischargeCurrent` | `uint16_t` | 0-999 (A) | |

### 2.25-2.32 QET / QEY / QEM / QED / QLT / QLY / QLM / QLD - `EnergyResponse`

Same struct for all eight commands (PV-generated vs. output-load energy, optionally scoped to a year/month/day passed as a request parameter) - only the query differs.

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `energyWh` | `uint32_t` | 0-99999999 (Wh) | 8-digit wire field |

## Setting commands (requests, protocol section 3 + 2.35)

Every request's `build()` writes its command payload (no CRC/`<cr>` yet) into a caller-supplied buffer; every reply is a plain `AckResponse` (`bool ok`) unless noted otherwise.

### 3.1 ATE1 - `StartAteTestRequest`

No fields - `build()` is `static`.

### 3.2 ATE0 - `EndAteTestRequest`

No fields - `build()` is `static`. *(Protocol's section title omits `<CRC><cr>`, unlike ATE1's - may need to be sent without CRC framing; not yet verified against real hardware.)*

### 3.3 PE / PD - `SetFlagsRequest`

Symmetric with `FlagStatusResponse` (2.9) - same 8 flags, each independently `Enabled`, `Disabled`, or left at the default `Unchanged`. Unlike every other Request struct, this one has two `build*()` methods instead of one (`buildEnableCommand()` / `buildDisableCommand()`), because the wire protocol itself is two separate commands (PE and PD) - `AxpertDevice::setFlags()` sends whichever of the two actually have something to say (0, 1, or both).

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `silenceBuzzer` | `AxpertFlagState` | see [Enums](#enums) | letter `a` |
| `overloadBypass` | `AxpertFlagState` | see [Enums](#enums) | letter `b` |
| `lcdEscapeToDefault` | `AxpertFlagState` | see [Enums](#enums) | letter `k` |
| `overloadRestart` | `AxpertFlagState` | see [Enums](#enums) | letter `u` |
| `overTemperatureRestart` | `AxpertFlagState` | see [Enums](#enums) | letter `v` |
| `backlightOn` | `AxpertFlagState` | see [Enums](#enums) | letter `x` |
| `alarmOnPrimarySourceInterrupt` | `AxpertFlagState` | see [Enums](#enums) | letter `y` |
| `faultCodeRecord` | `AxpertFlagState` | see [Enums](#enums) | letter `z` |

`AxpertDevice` also has one named convenience method per flag (e.g. `setBacklightOnEnabled(bool, ...)`) for changing exactly one flag without constructing the struct yourself.

### 3.4 PF - `ResetToDefaultsRequest`

No fields - `build()` is `static`.

### 3.5 MNCHGC - `SetMaxChargingCurrentRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `unitIndex` | `uint8_t` | 0-9 | parallel unit number; 0 for a single, non-parallel system |
| `currentAmps` | `uint16_t` | 0-999 (A) | must be one of the values `queryMaxChargingCurrentOptions()` returned |

### 3.6 MUCHGC - `SetMaxUtilityChargingCurrentRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `unitIndex` | `uint8_t` | 0-9 | parallel unit number; 0 for a single system |
| `currentAmps` | `uint16_t` | 0-999 (A) | must be one of the values `queryMaxUtilityChargingCurrentOptions()` returned |

### 3.7 F - `SetOutputFrequencyRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `frequencyHz` | `uint8_t` | 50 or 60 | |

### 3.8 POPV - `SetOutputRatingVoltageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltageTimesTen` | `uint16_t` | 2200, 2300, or 2400 | voltage already scaled by 10 (220/230/240 V) |

### 3.9 POP - `SetOutputSourcePriorityRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `priority` | `AxpertOutputSourcePriority` | see [Enums](#enums) | |

### 3.10 POPM - `SetOutputModeRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `mode` | `AxpertOutputMode` | see [Enums](#enums) | |

### 3.12 PBCV - `SetBatteryVoltageBackToUtilityRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.0-99.9 (V) | practical min/max depends on battery voltage (12/24/48V) and model - not enforced here |

### 3.13 PBDV - `SetBatteryVoltageBackToBatteryRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.0-99.9 (V) | `0.0` means "battery is full" (charging in float mode); practical range is battery/model-dependent |

### 3.14 PCP - `SetChargerSourcePriorityRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `priority` | `AxpertChargerSourcePriority` | see [Enums](#enums) | |

### 3.15 PPCP - `SetParallelChargerSourcePriorityRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `unitIndex` | `uint8_t` | 0-9 | parallel unit number |
| `priority` | `AxpertChargerSourcePriority` | see [Enums](#enums) | |

### 3.16 PGR - `SetGridWorkingRangeRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `range` | `AxpertInputVoltageRange` | see [Enums](#enums) | |

### 3.17 PBT - `SetBatteryTypeRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `type` | `AxpertBatteryType` | see [Enums](#enums) | |

### 3.18 PSDV - `SetBatteryCutOffVoltageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.0-99.9 (V) | battery/model-dependent practical range, not enforced here |

### 3.19 PCVV - `SetBatteryCvChargingVoltageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.0-99.9 (V) | battery/model-dependent practical range, not enforced here |

### 3.20 PBFT - `SetBatteryFloatChargingVoltageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.0-99.9 (V) | battery/model-dependent practical range, not enforced here |

### 3.21 / 3.22 BTA1 / BTA2 - `SetBatteryVoltageAdjustPointRequest`

One struct for both commands.

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `point` | `uint8_t` | 1 or 2 | picks BTA1 (1) or BTA2 (2); `build()` returns 0 for any other value |
| `voltage` | `float` | 0.00-999.99 (V) | |

### 3.23 BTA0 - `ResetBatteryVoltageAdjustRequest`

No fields - `build()` is `static`.

### 3.24 PPVOKC - `SetPvOkConditionRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `condition` | `AxpertPvOkConditionForParallel` | see [Enums](#enums) | |

### 3.25 PSPB - `SetPvPowerBalanceRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `balance` | `AxpertPvPowerBalance` | see [Enums](#enums) | |

### 3.26 RTEY - `ResetEnergyDataRequest`

No fields - `build()` is `static`.

### 3.27 RTDL - `EraseDataLogRequest`

No fields - `build()` is `static`.

### 3.28 PDL - `SetDataLogPeriodRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `minutes` | `uint8_t` | one of 3, 5, 10, 20, 30, 60 | not validated by `build()` - the device NAKs an out-of-range value |

### 3.29 PBEQE - `SetBatteryEqualizationEnabledRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `enabled` | `bool` | true/false | |

### 3.30 PBEQT - `SetBatteryEqualizationTimeRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `minutes` | `uint16_t` | 5-900 (in steps of 5) | not validated by `build()` |

### 3.31 PBEQP - `SetBatteryEqualizationPeriodRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `days` | `uint16_t` | 0-90 | not validated by `build()` |

### 3.32 PBEQV - `SetBatteryEqualizationVoltageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `voltage` | `float` | 0.00-99.99 (V) | battery/model-dependent practical range, not enforced here |

### 3.33 PBEQOT - `SetBatteryEqualizationOverTimeRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `minutes` | `uint16_t` | 5-900 (in steps of 5) | not validated by `build()` |

### 3.34 PBEQA - `SetBatteryEqualizationActiveRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `active` | `bool` | true/false | |

### 3.35 PCVT - `SetMaxChargingTimeAtCvRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `minutes` | `uint16_t` | 0-999 | |

### 2.35 PBMS - `SetBmsMessageRequest`

| Field | Type | Range / Values | Notes |
|---|---|---|---|
| `disconnectBattery` | `bool` | false = connect, true = disconnect | letter `a` |
| `batteryPercent` | `uint16_t` | 0-999 (%) | letters `bbb` |
| `forceAcCharge` | `bool` | true/false | letter `c`: force AC charge in any case |
| `disableDischarge` | `bool` | true/false | letter `d`: battery stop discharge flag |
| `disableCharge` | `bool` | true/false | letter `e`: battery stop charge flag |
| `cvChargingVoltage` | `uint16_t` | 0-999 (V) | letters `fff`; documented as a plain integer, no decimal point |
| `floatingChargingVoltage` | `uint16_t` | 0-999 (V) | letters `ggg` |
| `cutOffVoltage` | `uint16_t` | 0-999 (V) | letters `hhh` |
| `maxChargingCurrent` | `uint16_t` | 0-9999 (A) | letters `iiii` |
| `maxDischargingCurrent` | `uint16_t` | 0-9999 (A) | letters `jjjj` |
