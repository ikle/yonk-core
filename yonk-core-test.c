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

#include <sys/time.h>

#include <yonk/core.h>

static long get_us (void)
{
	struct timeval tv;

	if (gettimeofday (&tv, NULL) != 0)
		return -1;

	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main (int argc, char *argv[])
{
	struct yonk *o;
	clock_t ts;
	long us;
	long i, k, t, a, *v;
	struct yonk_node *n;

	if ((o = yonk_alloc ("yonk-core.db", "w")) == NULL) {
		fprintf (stderr, "E: cannot connect to database\n");
		return 1;
	}

	ts = clock (); us = get_us ();

	yonk_discard (o);

	i = yonk_add (o, 0, 0, "interfaces",	YONK_GROUP, 0);
	i = yonk_add (o, i, 0, "ethernet",	YONK_NODE,  0);
	t = yonk_add (o, i, 0, "eth1",		YONK_TAG,   0);
	a = yonk_add (o, t, 0, "address",	YONK_ATTR,  0);
	i = yonk_add (o, a, 0, "10.0.26.2/24",	YONK_VALUE, 0);
	k = yonk_add (o, a, 0, "10.0.27.2/24",	YONK_VALUE, 0);
	i = yonk_add (o, a, 0, "10.0.28.2/24",	YONK_VALUE, 0);
	i = yonk_add (o, a, 0, "10.0.24.2/24",	YONK_VALUE, 0);

	yonk_delete (o, k);

	if ((v = yonk_childs (o, a, 1)) != NULL)
		for (i = 0; v[i] > 0; ++i) {
			printf ("address value (%ld) = ", v[i]);

			if ((n = yonk_get (o, v[i])) != NULL)
				printf ("%s", n->label);

			printf ("\n");
		}

	free (v);

	yonk_commit (o);
	yonk_delete (o, a);

	a = yonk_add (o, t, 0, "description",	YONK_ATTR,  0);
	i = yonk_add (o, a, 0, "Cool link",	YONK_VALUE, 0);

	ts = clock () - ts; us = get_us () - us;

	printf ("I: requests completed in %.3f ms (CPU time), "
		"%.3f ms (real time)\n",
		ts * (1000.0 / CLOCKS_PER_SEC), us * 0.001);

	yonk_free (o);
	return 0;
}
