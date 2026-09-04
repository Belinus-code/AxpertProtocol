// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// QPI (2.1): "(PI<NN>" - device protocol ID (30 for Axpert series).
struct ProtocolIdResponse {
    uint8_t protocolId; // NN

    static bool parse(const uint8_t* raw, size_t len, ProtocolIdResponse& out);
};
