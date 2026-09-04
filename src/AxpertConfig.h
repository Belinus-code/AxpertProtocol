// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

// Default timeout (in milliseconds) used for a request/response transaction
// when no per-call timeout is given. Overridable per AxpertDevice instance
// (constructor / setDefaultTimeout()) or per call (every query*()/set*()
// method takes an optional timeoutMs override).
#ifndef AXPERT_DEFAULT_TIMEOUT_MS
#define AXPERT_DEFAULT_TIMEOUT_MS 1000
#endif

// Maximum length of a raw request/response frame (payload + CRC + <cr>).
// The longest documented replies (QPIGS/QPIRI) fit comfortably under this.
#ifndef AXPERT_MAX_FRAME_LEN
#define AXPERT_MAX_FRAME_LEN 128
#endif

// ---------------------------------------------------------------------------
// Transport mode switch (compile-time only, no runtime cost, NOT implemented
// yet - this is just the seam for later).
//
// Only the blocking transport exists right now (AxpertTransport::transact()
// writes the request and blocks until a full frame arrives or timeoutMs
// elapses). AxpertTransport is deliberately kept in its own translation unit
// and behind a narrow interface (see AxpertTransport.h) so that a future
// non-blocking, state-machine-driven transport could be added and selected
// here - e.g.:
//
//   #define AXPERT_TRANSPORT_BLOCKING    1
//   #define AXPERT_TRANSPORT_NONBLOCKING 2
//   #ifndef AXPERT_TRANSPORT_MODE
//   #define AXPERT_TRANSPORT_MODE AXPERT_TRANSPORT_BLOCKING
//   #endif
//
// - without changing AxpertDevice's public API or any response parser.
// ---------------------------------------------------------------------------
