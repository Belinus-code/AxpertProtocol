// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QVFW / QVFW2 / QVFW3 (2.4-2.6): "(VERFW:NNNNN.NN" or "(VERFW2:NNNNN.NN" - a series
// number and a version, both encoded as hex digits per the protocol.
struct FirmwareVersionResponse {
    uint32_t seriesNumber; // NNNNN
    uint8_t version;       // NN

    static bool parse(const uint8_t* raw, size_t len, FirmwareVersionResponse& out);
};
