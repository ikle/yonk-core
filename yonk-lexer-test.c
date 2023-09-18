/*
 * Yonk Configuration Specification Lexer Test
 *
 * Copyright (c) 2022-2023 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>

#include "yonk-lexer.h"

static const char *yt_name (struct yonk_token *o)
{
	switch (o->type) {
	case YT_LF:		return "lf";
	case YT_STAR:		return "star";
	case YT_COMMA:		return "comma";
	case YT_COLON:		return "colon";
	case YT_EQU:		return "equ";
	case YT_LBR:		return "lbr";
	case YT_RBR:		return "rbr";
	case YT_UINT:		return "uint";
	case YT_NAME:		return "name";
	case YT_STRING:		return "string";
	}

	return o->value;
}

int main (int argc, char *argv[])
{
	struct yonk_slex *o;
	struct yonk_token *tok;

	if (argc != 2) {
		fprintf (stderr, "usage:\n\tyonk-lexer-test <spec-file>\n");
		return 1;
	}

	if ((o = yonk_slex_open (argv[1], 128)) == NULL) {
		perror ("E: spec open");
		return 1;
	}

	while ((tok = yonk_slex_get (o)) != NULL)
		if (tok->type == YT_ERROR)
			goto error;
		else
		if (tok->type < YT_UINT)
			printf ("I: %s\n", yt_name (tok));
		else
			printf ("I: %s : %s\n", yt_name (tok), tok->value);

	yonk_slex_close (o);
	return 0;
error:
	fprintf (stderr, "E: %s\n", tok->value);
	yonk_slex_close (o);
	return 1;
}
