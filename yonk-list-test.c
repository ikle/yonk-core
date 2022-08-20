/*
 * Yonk Core Listing test
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>

#include <yonk/core.h>

int main (int argc, char *argv[])
{
	struct yonk *o;

	if ((o = yonk_alloc ("yonk-core.db", "w")) == NULL) {
		fprintf (stderr, "E: cannot connect to database\n");
		return 1;
	}

	yonk_list (o, 0, 0, stdout);

	yonk_free (o);
	return 0;
}
