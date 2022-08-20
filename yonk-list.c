/*
 * Yonk Core Listing
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <yonk/core.h>

static int get_state (const struct yonk_node *o)
{
	return !o->dirty ? ' ' : o->active ? '-' : '+';
}

static void indent (int state, int level, FILE *to)
{
	fprintf (to, "%c", state);

	for (; level > 0; --level)
		fprintf (to, "    ");
}

static void show_label (const char *key, const char *label, int leaf, FILE *to)
{
	size_t i = strcspn (label, " \"\\");

	if (key != NULL)
		fprintf (to, "%s ", key);

	if (label[i] == '\0')
		fprintf (to, "%s", label);
	else {
		fputc ('"', to);

		for (i = 0; label[i] != '\0'; fputc (label[i], to), ++i)
			if (label[i] == '"' || label[i] == '\\')
				fputc ('\\', to);

		fputc ('"', to);
	}

	fprintf (to, leaf ? "\n" : " {\n");
}

static int
yonk_list_attrs (struct yonk *o, int level, long attr, const char *name, FILE *to)
{
	long *v, i;
	struct yonk_node *n;

	if ((v = yonk_childs (o, attr, 1)) == NULL)
		return 0;

	for (i = 0; v[i] > 0; ++i) {
		if ((n = yonk_get (o, v[i])) == NULL || n->kind != YONK_VALUE)
			continue;

		indent (get_state (n), level, to);
		show_label (name, n->label, 1, to);
	}

	free (v);
	return 1;
}

static int
yonk_list_nodes (struct yonk *o, int level, long node, const char *name, FILE *to)
{
	long *v, i;
	struct yonk_node *n;
	int s;

	if ((v = yonk_childs (o, node, 1)) == NULL)
		return 0;

	for (i = 0; v[i] > 0; ++i) {
		if ((n = yonk_get (o, v[i])) == NULL || n->kind != YONK_TAG)
			continue;

		indent (s = get_state (n), level, to);
		show_label (name, n->label, 0, to);
		yonk_list (o, level + 1, v[i], to);
		indent (s, level, to);
		fprintf (to, "}\n");
	}

	free (v);
	return 1;
}

int yonk_list (struct yonk *o, int level, long parent, FILE *to)
{
	long *v, i;
	struct yonk_node *n;
	char *name;
	int s;

	if ((v = yonk_childs (o, parent, 1)) == NULL)
		return 0;

	for (i = 0; v[i] > 0; ++i) {
		if ((n = yonk_get (o, v[i])) == NULL)
			continue;

		switch (n->kind) {
		case YONK_GROUP:
			indent (s = get_state (n), level, to);
			show_label (NULL, n->label, 0, to);
			yonk_list (o, level + 1, v[i], to);
			indent (s, level, to);
			fprintf (to, "}\n");
			break;
		case YONK_NODE:
			name = strdup (n->label);
			yonk_list_nodes (o, level, v[i], name, to);
			free (name);
			break;
		case YONK_ATTR:
			name = strdup (n->label);
			yonk_list_attrs (o, level, v[i], name, to);
			free (name);
			break;
		}
	}

	free (v);
	return 1;
}
