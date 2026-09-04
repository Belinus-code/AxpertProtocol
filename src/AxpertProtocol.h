// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

// Single include for library users: brings in the device class, all
// response structs, shared enums and the CRC helper.
#include "AxpertConfig.h"
#include "AxpertCRC.h"
#include "AxpertDevice.h"
#include "AxpertTypes.h"
#include "responses/AckResponse.h" // not used by AxpertDevice.h itself anymore
                                    // (see AxpertDevice.cpp), but still a
                                    // useful public type for anyone parsing
                                    // a raw ACK/NAK reply by hand
