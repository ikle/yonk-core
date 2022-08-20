/*
 * SQLite High-Level Interface
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdarg.h>

#include "sqlite-hli.h"

int sqlite_compile (sqlite3 *o, const char *req, sqlite3_stmt **s)
{
	return *s != NULL || sqlite3_prepare_v2 (o, req, -1, s, NULL) == 0;
}

int sqlite_bind_va (sqlite3_stmt *s, const char *fmt, va_list ap)
{
	int i;

	if (sqlite3_reset (s) != 0)
		return 0;

	for (i = 1; *fmt != '\0'; ++fmt, ++i)
		switch (*fmt) {
		case 'i': {
				int x = va_arg (ap, int);

				if (sqlite3_bind_int (s, i, x) != 0)
					return 0;

				break;
			}
		case 'l': {
				long x = va_arg (ap, long);

				if (sqlite3_bind_int64 (s, i, x) != 0)
					return 0;

				break;
			}
		case 's': {
				const char *x = va_arg (ap, const char *);

				if (sqlite3_bind_text (s, i, x, -1, NULL) != 0)
					return 0;

				break;
			}
		}

	return 1;
}

int sqlite_bind (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok;
}

int sqlite_run (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok && sqlite3_step (s) == SQLITE_DONE;
}

int sqlite_first (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok && sqlite3_step (s) == SQLITE_ROW;
}

int sqlite_next (sqlite3_stmt *s)
{
	return sqlite3_step (s) == SQLITE_ROW;
}
