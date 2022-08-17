/*
 * Yonk Core: Low-Level Configuration API
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef YONK_CORE_H
#define YONK_CORE_H  1

#include <stdio.h>

enum yonk_kind {
	YONK_GROUP,
	YONK_NODE,
	YONK_TAG,
	YONK_ATTR,
	YONK_VALUE,
};

struct yonk_node {
	long parent, link;
	const char *label;
	unsigned char kind, secure;
	unsigned char active, dirty, changed;
};

struct yonk *yonk_alloc (const char *path, const char *mode);
void yonk_free (struct yonk *o);

/* access */

struct yonk_node *yonk_get (struct yonk *o, long id);
long  yonk_get_parent (struct yonk *o, long id);
long  yonk_lookup (struct yonk *o, long parent, const char *label, int active);
long *yonk_childs (struct yonk *o, long parent, int sorted);
long *yonk_slaves (struct yonk *o, long id);

/* modify */

long yonk_add (struct yonk *o, long parent, long link, const char *label,
	       int kind, int secure);
int yonk_delete  (struct yonk *o, long id);
int yonk_commit  (struct yonk *o);
int yonk_discard (struct yonk *o);

/* path */

typedef int yonk_node_cb (struct yonk_node *n, void *cookie);

int yonk_iterate_path (struct yonk *o, long id, yonk_node_cb cb, void *cookie);
int yonk_show_path    (struct yonk *o, long id, FILE *to);

#endif  /* YONK_CORE_H */
