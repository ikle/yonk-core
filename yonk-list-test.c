/*
 * Yonk Core Listing test
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <time.h>

#include <yonk/core.h>

int main (int argc, char *argv[])
{
	struct yonk *o;
	clock_t ts;

	if ((o = yonk_alloc ("yonk-core.db", "r")) == NULL) {
		fprintf (stderr, "E: cannot connect to database\n");
		return 1;
	}

	ts = clock ();

	yonk_list (o, 0, 0, 0, stdout);

	ts = clock () - ts;

	printf ("I: requests completed in %.3f ms\n",
		ts * (1000.0 / CLOCKS_PER_SEC));

	yonk_free (o);
	return 0;
}
