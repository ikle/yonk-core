/*
 * SQLite High-Level Interface
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SQLITE_HLI_H
#define SQLITE_HLI_H  1

#include <stddef.h>

#include <sqlite3.h>

int sqlite_compile (sqlite3 *o, const char *req, sqlite3_stmt **s);

int sqlite_bind  (sqlite3_stmt *s, const char *fmt, ...);
int sqlite_run   (sqlite3_stmt *s, const char *fmt, ...);
int sqlite_first (sqlite3_stmt *s, const char *fmt, ...);
int sqlite_next  (sqlite3_stmt *s);

#endif  /* SQLITE_HLI_H */
