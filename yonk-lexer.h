/*
 * Yonk Configuration Specification Lexer
 *
 * Copyright (c) 2022-2023 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef YONK_LEXER_H
#define YONK_LEXER_H  1

#include <stddef.h>

enum yonk_token_type {
	YT_ERROR	= 0,		/* unexpected input	*/

	YT_LF		= 012,		/* \n			*/
	YT_STAR		= 052,		/* *			*/
	YT_COMMA	= 054,		/* ,			*/
	YT_COLON	= 072,		/* :			*/
	YT_EQU		= 075,		/* =			*/
	YT_LBR		= 0173,		/* {			*/
	YT_RBR		= 0175,		/* }			*/

	YT_GROUP	= 0200,		/* group		*/
	YT_NODES,			/* nodes		*/
	YT_NODE,			/* node			*/
	YT_ATTRS,			/* attrs		*/
	YT_ATTR,			/* attr			*/
	YT_VALUE,			/* value		*/
	YT_NUMBER,			/* number		*/
	YT_FROM,			/* from			*/
	YT_TO,				/* to			*/
	YT_MATCH,			/* match		*/
	YT_REF,				/* ref			*/
	YT_CLASS,			/* class		*/

	YT_UINT		= 0300,		/* 0|([1-9][0-9]*)	*/
	YT_NAME,			/* [a-z](-?[0-9a-z])*	*/
	YT_STRING,			/* "([^"\\]|(\\.))*"	*/
};

struct yonk_token {
	int type;
	const char *value;
};

struct yonk_slex *yonk_slex_open (const char *path, size_t limit);
void yonk_slex_close (struct yonk_slex *o);

struct yonk_token *yonk_slex_get (struct yonk_slex *o);

#endif  /* YONK_LEXER_H */
