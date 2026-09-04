// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>

#include "../AxpertTypes.h"

// QMOD (2.11): "(M" - single-character device mode code.
struct DeviceModeResponse {
    AxpertDeviceMode mode;

    static bool parse(const uint8_t* raw, size_t len, DeviceModeResponse& out);
};
