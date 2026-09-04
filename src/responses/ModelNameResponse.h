// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

#define AXPERT_MODEL_NAME_MAX_LEN 16

// QMN (2.20): "(MMMMM-NNNN" - model name + rated output VA.
struct ModelNameResponse {
    char name[AXPERT_MODEL_NAME_MAX_LEN + 1];
    uint16_t ratedOutputVa;

    static bool parse(const uint8_t* raw, size_t len, ModelNameResponse& out);
};

// QGMN (2.21): "(NNN" - numeric general model code (see protocol 2.21
// table for the full mapping, e.g. 022 = "AXPERT MKS II 5KW").
struct GeneralModelNameResponse {
    uint16_t modelCode;

    static bool parse(const uint8_t* raw, size_t len, GeneralModelNameResponse& out);
};
