// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "EnergyResponse.h"

#include "../AxpertParsing.h"

bool EnergyResponse::parse(const uint8_t* raw, size_t len, EnergyResponse& out) {
    // "(NNNNNNNN" - 8 decimal digits.
    if (len != 9 || raw[0] != '(') return false;
    out.energyWh = AxpertParsing::parseUIntToken(reinterpret_cast<const char*>(raw) + 1, 8);
    return true;
}
