// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// Generic (ACK<cr> / NAK<cr>) reply used by most setting commands (3.x).
struct AckResponse {
    bool ok;

    static bool parse(const uint8_t* raw, size_t len, AckResponse& out);
};
