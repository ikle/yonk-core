/*
 * SQLite High-Level Interface
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "sqlite-hli.h"

int sqlite_compile (sqlite3 *o, const char *req, sqlite3_stmt **s)
{
	if (*s == NULL && sqlite3_prepare_v2 (o, req, -1, s, NULL) != 0)
		return 0;

	return sqlite3_reset (*s) == 0;
}
