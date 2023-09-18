/*
 * Yonk Configuration Specification Lexer
 *
 * Copyright (c) 2022-2023 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yonk-lexer.h"

struct yonk_slex {
	FILE *f;
	size_t limit;
	long lineno;
	struct yonk_token tok;
	char line[1];		/* reserved extra one for '\0' */
};

struct yonk_slex *yonk_slex_open (const char *path, size_t limit)
{
	struct yonk_slex *o;

	if ((o = malloc (sizeof (*o) + limit)) == NULL)
		return o;

	if ((o->f = fopen (path, "rb")) == NULL)
		goto no_open;

	o->limit	= limit;
	o->lineno 	= 1;
	o->tok.value	= o->line;
	return o;
no_open:
	free (o);
	return NULL;
}

void yonk_slex_close (struct yonk_slex *o)
{
	if (o == NULL)
		return;

	fclose (o->f);
	free (o);
}

static int yt_get (struct yonk_slex *o)
{
	int a;

	while ((a = fgetc (o->f)) == 011 || a == 040) {}  /* HT or SPACE */

	if (a == 043)  /* # */
		while ((a = fgetc (o->f)) != EOF && a != YT_LF) {}

	if (a == YT_LF)
		++o->lineno;

	return a;
}

static struct yonk_token *yt_get_string (struct yonk_slex *o)
{
	size_t i;
	int a;

	for (i = 0; i < o->limit; o->line[i++] = a) {
		if ((a = fgetc (o->f)) == 042)   /* quote char */
			break;

		if (a == 0134)  /* backslash */
			a = fgetc (o->f);

		if (a == EOF)
			goto on_eof;
	}

	o->line[i] = '\0';
	o->tok.type = YT_STRING;
	return &o->tok;
on_eof:
	snprintf (o->line, o->limit + 1,
		  "E: Unexpected EOF in string token at line %ld\n", o->lineno);
	o->tok.type = YT_ERROR;
	return &o->tok;
}

static struct yonk_token *yt_get_uint (struct yonk_slex *o, int first)
{
	size_t i;
	int a;

	for (o->line[0] = first, i = 1; i < o->limit; o->line[i++] = a)
		if ((a = fgetc (o->f)) < 060 || a > 071) {
			if (a != EOF)
				ungetc (a, o->f);
			break;
		}

	o->line[i] = '\0';
	o->tok.type = YT_UINT;
	return &o->tok;
}

static int is_alpha (int a)
{
	return (a >= 0141 && a <= 0172);
}

static int is_alnum (int a)
{
	return (a >= 060 && a <= 071) || is_alpha (a);
}

static struct yonk_token yt_keyword[] = {
	{ YT_GROUP,	"group"		},
	{ YT_NODES,	"nodes"		},
	{ YT_NODE,	"node"		},
	{ YT_ATTRS,	"attrs"		},
	{ YT_ATTR,	"attr"		},
	{ YT_VALUE,	"value"		},
	{ YT_NUMBER,	"number"	},
	{ YT_FROM,	"from"		},
	{ YT_TO,	"to"		},
	{ YT_MATCH,	"match"		},
	{ YT_REF,	"ref"		},
	{ YT_CLASS,	"class"		},
	{ YT_ERROR,	NULL		},
};

static struct yonk_token *yt_get_name (struct yonk_slex *o, int first)
{
	size_t i;
	int a;

	for (o->line[0] = first, i = 1; i < o->limit; o->line[i++] = a) {
		if ((a = fgetc (o->f)) == 055) {  /* dash */
			o->line[i++] = a;

			if (i < o->limit && is_alnum (a = fgetc (o->f)))
				continue;

			goto error;
		}

		if (!is_alnum (a)) {
			ungetc (a, o->f);
			break;
		}
	}

	o->line[i] = '\0';

	for (i = 0; yt_keyword[i].type != YT_ERROR; ++i)
		if (strcmp (o->line, yt_keyword[i].value) == 0)
			return yt_keyword + i;

	o->tok.type = YT_NAME;
	return &o->tok;
error:
	snprintf (o->line, o->limit + 1,
		  "E: Name token expected at line %ld\n", o->lineno);
	o->tok.type = YT_ERROR;
	return &o->tok;
}

struct yonk_token *yonk_slex_get (struct yonk_slex *o)
{
	int a;

	switch ((a = yt_get (o))) {
	case EOF:
		return NULL;
	case YT_LF: case YT_STAR: case YT_COMMA: case YT_COLON:
	case YT_EQU: case YT_LBR: case YT_RBR:
		o->tok.type = a;
		return &o->tok;
	case 042:
		return yt_get_string (o);
	case 060:
		o->line[0] = '0';
		o->line[1] = '\0';
		o->tok.type = YT_UINT;
		return &o->tok;
	case 061: case 062: case 063: case 064: case 065: case 066:
	case 067: case 070: case 071:
		return yt_get_uint (o, a);
	}

	if (is_alpha (a))
		return yt_get_name (o, a);

	snprintf (o->line, o->limit + 1,
		  "E: Unexpected input at line %ld\n", o->lineno);
	o->tok.type = YT_ERROR;
	return &o->tok;
}
