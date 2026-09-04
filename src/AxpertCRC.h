// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// CRC-16/XMODEM (poly 0x1021, init 0x0000) as used to frame every Axpert
// request/response, plus the protocol's byte-escaping quirk (appendix 4.1):
// if a computed CRC byte would equal '(' (0x28), <cr> (0x0D) or <lf> (0x0A),
// it will be incremented by one - otherwise a receiver
// scanning for those bytes could misframe the message.
namespace AxpertCRC
{

    // Computes the raw CRC-16/XMODEM over data[0..len). Checks for the
    // escaping quirk and increments any CRC byte that would equal
    //  '(' (0x28), <cr> (0x0D) or <lf> (0x0A) by one. Returns the 16-bit CRC value.
    uint16_t compute(const uint8_t *data, size_t len);

    // Appends the 2 CRC bytes (with the escaping quirk applied) plus a
    // trailing <cr> (0x0D) to `frame`, whose first `payloadLen` bytes already
    // hold the command/reply payload. `frameCapacity` is the
    // total size of the `frame` buffer.
    // Returns the total frame length written (payloadLen + 3), or 0 if
    // `frame` was too small.
    size_t appendFramed(uint8_t *frame, size_t frameCapacity, size_t payloadLen);

    // Verifies the trailing 2-byte CRC of a received frame (payload + CRC). Returns true if valid.
    bool verify(const uint8_t *frame, size_t frameLenWithoutCr);

    // CRC-16/XMODEM lookup table for the 4-bit-at-a-time algorithm.
    extern const uint16_t crc_table[16];

} // namespace AxpertCRC
