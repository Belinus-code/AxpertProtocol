// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#include "AxpertTransport.h"

AxpertTransport::AxpertTransport(Stream &port) : _port(&port) {}

size_t AxpertTransport::transact(const uint8_t *cmd, size_t cmdLen,
                                 uint8_t *respBuf, size_t respBufCapacity,
                                 uint16_t timeoutMs)
{
    _port->write(cmd, cmdLen);
    return readFrame(respBuf, respBufCapacity, timeoutMs);
}

size_t AxpertTransport::readFrame(uint8_t *buf, size_t capacity, uint16_t timeoutMs)
{
    uint32_t startTime = millis();
    size_t bytesRead = 0;
    while (millis() - startTime < timeoutMs)
    {
        if (_port->available())
        {
            int byteRead = _port->read();
            if (byteRead == -1)
            {
                // Stream read error
                break;
            }
            if (byteRead == 0x0D)
            {
                // End of frame
                return bytesRead;
            }
            if (capacity > 0)
            {
                *buf++ = static_cast<uint8_t>(byteRead);
                capacity--;
                bytesRead++;
                startTime = millis(); // Reset timeout on successful read
            }
            else
            {
                // Buffer full
                break;
            }
        }
        yield(); // Allow other tasks (like a watchdog) to run
    }
    // Timeout or error
    return 0;
}
