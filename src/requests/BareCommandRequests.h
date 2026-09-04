// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

// A handful of section-3 commands take no parameters at all - just their
// literal command name. Grouped in one file since each struct below is
// otherwise a repeat of the same trivial build() shape.

// 3.1 ATE1<CRC><cr>: start ATE test, remote panel stops polling.
struct StartAteTestRequest {
    static size_t build(char* buf, size_t bufCapacity);
};

// 3.2 ATE0: end ATE test, remote panel resumes polling. The protocol's own
// title for this one omits "<CRC><cr>" (unlike 3.1's), so this may need to
// be sent without CRC framing - verify against a real device.
struct EndAteTestRequest {
    static size_t build(char* buf, size_t bufCapacity);
};

// 3.4 PF<cr>: reset all setting parameters to their default values.
struct ResetToDefaultsRequest {
    static size_t build(char* buf, size_t bufCapacity);
};

// 3.23 BTA0<cr>: reset the battery voltage adjust points (BTA1/BTA2) to
// their default values.
struct ResetBatteryVoltageAdjustRequest {
    static size_t build(char* buf, size_t bufCapacity);
};

// 3.26 RTEY<cr>: reset all stored PV/load energy data.
struct ResetEnergyDataRequest {
    static size_t build(char* buf, size_t bufCapacity);
};

// 3.27 RTDL<cr>: erase all data log entries.
struct EraseDataLogRequest {
    static size_t build(char* buf, size_t bufCapacity);
};
