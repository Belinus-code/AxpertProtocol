// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AxpertCRC.h"

namespace AxpertCRC
{
    const uint16_t crc_table[16] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};

    uint16_t compute(const uint8_t *data, size_t len)
    {
        uint16_t crc = 0x0000;
        for (size_t i = 0; i < len; i++)
        {
            uint8_t da = ((crc >> 8) >> 4);
            crc <<= 4;
            crc ^= crc_table[da ^ (data[i] >> 4)];
            da = ((crc >> 8) >> 4);
            crc <<= 4;
            crc ^= crc_table[da ^ (data[i] & 0x0F)];
        }

        uint8_t crc_low = crc & 0xFF;
        uint8_t crc_high = (crc >> 8) & 0xFF;

        if (crc_low == 0x28 || crc_low == 0x0D || crc_low == 0x0A)
            crc_low++;
        if (crc_high == 0x28 || crc_high == 0x0D || crc_high == 0x0A)
            crc_high++;

        return (uint16_t)((crc_high << 8) | crc_low);
    }

    size_t appendFramed(uint8_t *frame, size_t frameCapacity, size_t payloadLen)
    {
        if (payloadLen + 3 > frameCapacity)
        {
            return 0;
        }
        uint16_t crc = compute(frame, payloadLen);
        uint8_t crc_low = crc & 0xFF;
        uint8_t crc_high = (crc >> 8) & 0xFF;
        frame[payloadLen] = crc_high;
        frame[payloadLen + 1] = crc_low;
        frame[payloadLen + 2] = 0x0D; // Append <cr>
        return payloadLen + 3;
    }

    bool verify(const uint8_t *frame, size_t frameLenWithoutCr)
    {
        uint16_t received_crc = (frame[frameLenWithoutCr - 2] << 8) | frame[frameLenWithoutCr - 1];
        if (compute(frame, frameLenWithoutCr - 2) == received_crc)
        {
            return true;
        }
        return false;
    }

} // namespace AxpertCRC
