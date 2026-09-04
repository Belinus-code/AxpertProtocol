// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

// Wraps a single Stream (HardwareSerial, SoftwareSerial, ...) and performs
// request/response transactions for the Axpert protocol.
//
// This implementation is synchronous/blocking: transact() writes the
// request and then blocks (up to timeoutMs) until a full frame has been
// read or the timeout expires.
//
// It is deliberately kept in its own translation unit, behind this same
// interface, so a future non-blocking implementation (a state machine
// driven from a poll()/update() call instead of blocking inside transact())
// could be dropped in - selected via the AXPERT_TRANSPORT_MODE switch in
// AxpertConfig.h - without changing AxpertDevice or any response parser.
// Nothing about that is implemented yet; this is just the seam for it.
class AxpertTransport {
public:
    explicit AxpertTransport(Stream& port);

    // Writes `cmd` (cmdLen bytes, already including CRC + trailing <cr>),
    // then reads a response into `respBuf` (up to respBufCapacity bytes)
    // until <cr> is seen or `timeoutMs` elapses since the last received
    // byte. Returns the number of bytes read into respBuf (0 on
    // timeout/error).
    size_t transact(const uint8_t* cmd, size_t cmdLen,
                     uint8_t* respBuf, size_t respBufCapacity,
                     uint16_t timeoutMs);

private:
    Stream* _port;

    // Reads bytes from _port into buf until <cr> (0x0D) is seen, `capacity`
    // is reached, or `timeoutMs` elapses. Returns the number of bytes read
    // (not including the terminating <cr>).
    size_t readFrame(uint8_t* buf, size_t capacity, uint16_t timeoutMs);
};
