/*
 * Yonk Core test
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <yonk/core.h>

int main (int argc, char *argv[])
{
	struct yonk *o;
	clock_t ts;
	long i, j, k, *v;
	struct yonk_node *n;

	if ((o = yonk_alloc ("yonk-core.db", "w")) == NULL) {
		fprintf (stderr, "E: cannot connect to database\n");
		return 1;
	}

	ts = clock ();

	yonk_discard (o);

	i = yonk_add (o, 0, 0, "interfaces",	YONK_GROUP, 0);
	i = yonk_add (o, i, 0, "ethernet",	YONK_NODE,  0);
	i = yonk_add (o, i, 0, "eth1",		YONK_TAG,   0);
	j = yonk_add (o, i, 0, "address",	YONK_ATTR,  0);
	i = yonk_add (o, j, 0, "10.0.26.2/24",	YONK_VALUE, 0);
	k = yonk_add (o, j, 0, "10.0.27.2/24",	YONK_VALUE, 0);
	i = yonk_add (o, j, 0, "10.0.28.2/24",	YONK_VALUE, 0);
	i = yonk_add (o, j, 0, "10.0.24.2/24",	YONK_VALUE, 0);

	yonk_delete (o, k);

	if ((v = yonk_childs (o, j, 1)) != NULL)
		for (i = 0; v[i] > 0; ++i) {
			printf ("address value (%ld) = ", v[i]);

			if ((n = yonk_get (o, v[i])) != NULL)
				printf ("%s", n->label);

			printf ("\n");
		}

	free (v);

	yonk_commit (o);
	yonk_delete (o, j);

	ts = clock () - ts;

	printf ("I: requests completed in %.3f ms\n",
		ts * (1000.0 / CLOCKS_PER_SEC));

	yonk_free (o);
	return 0;
}
