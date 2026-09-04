// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2026 Linus Meinders

#pragma once

#include <stddef.h>
#include <stdint.h>

// Shared helpers for parsing the space-separated ASCII responses used
// throughout the Axpert protocol (QPIGS, QPIRI, QDI, ...). Every
// Response::parse() expects `raw` to still include the leading '(' - the
// CRC and trailing <cr> are already stripped by the caller.
namespace AxpertParsing {

// Advances `cursor` past a single space-separated token in [cursor, end).
// On success, `tokenStart`/`tokenLen` describe the token (not
// null-terminated) and `cursor` is left just past it. Returns false once
// no more tokens remain.
bool nextToken(const char*& cursor, const char* end, const char*& tokenStart, size_t& tokenLen);

// Parses a decimal token (optionally containing a '.') into a float.
float parseFloatToken(const char* token, size_t len);

// Parses a token of decimal digits into an unsigned integer. Stops at the
// first non-digit character.
uint32_t parseUIntToken(const char* token, size_t len);

// Copies a token into `dest` (capacity `destCapacity`, including the
// terminating null) and null-terminates it. Returns false if the token
// didn't fit (the copy is truncated in that case, not skipped).
bool copyToken(const char* token, size_t len, char* dest, size_t destCapacity);

} // namespace AxpertParsing
