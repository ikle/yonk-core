/*
 * Yonk Core: Low-Level Configuration API
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include <yonk/core.h>

#include "sqlite-hli.h"

struct yonk {
	sqlite3 *db;
	struct yonk_node node;
	sqlite3_stmt *get, *parent, *lookup;
	sqlite3_stmt *nchilds, *childs, *childs_s, *nslaves, *slaves;
	sqlite3_stmt *mark, *add, *del_u, *del_d;
	sqlite3_stmt *comm_d, *comm_u, *dis_d, *dis_u;
};

static const char *init_req =
	"CREATE TABLE IF NOT EXISTS tree ("
	"	id	INTEGER PRIMARY KEY,"
	"	parent	INTEGER NOT NULL,"
	"	link	INTEGER NOT NULL DEFAULT 0,"
	"	label	TEXT    NOT NULL,"
	"	kind	INTEGER NOT NULL,"
	"	secure	BOOLEAN NOT NULL DEFAULT 0," /* secure key */

	"	active	BOOLEAN NOT NULL DEFAULT 0," /* committed */
	"	dirty	BOOLEAN NOT NULL DEFAULT 1," /* will be added/deleted */
	"	changed	BOOLEAN NOT NULL DEFAULT 1," /* validate requred */

	"	UNIQUE (parent, label)"
	");"

	"CREATE INDEX IF NOT EXISTS \"tree-parent\" ON tree (parent);"
	"CREATE INDEX IF NOT EXISTS \"tree-link\"   ON tree (link)";

struct yonk *yonk_alloc (const char *path, const char *mode)
{
	struct yonk *o;
	const char *p;
	int flags = 0;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	for (p = mode; *p != '\0'; ++p)
		switch (*p) {
		case 'r':
			flags |= SQLITE_OPEN_READONLY;
			break;
		case 'w':
			flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
			break;
		}

	if (sqlite3_open_v2 (path, &o->db, flags, NULL) != 0)
		goto no_db;

	if (sqlite3_exec (o->db, init_req, NULL, NULL, NULL) != 0)
		goto no_init;

	o->lookup = o->parent = o->get = NULL;
	o->slaves = o->nslaves = o->childs_s = o->childs = o->nchilds = NULL;
	o->del_d = o->del_u = o->add = o->mark = NULL;
	o->dis_u = o->dis_d = o->comm_u = o->comm_d = NULL;
	return o;
no_init:
	sqlite3_close_v2 (o->db);
no_db:
	free (o);
	return NULL;
}

void yonk_free (struct yonk *o)
{
	if (o == NULL)
		return;

	sqlite3_finalize (o->get);
	sqlite3_finalize (o->parent);
	sqlite3_finalize (o->lookup);
	sqlite3_finalize (o->nchilds);
	sqlite3_finalize (o->childs);
	sqlite3_finalize (o->childs_s);
	sqlite3_finalize (o->nslaves);
	sqlite3_finalize (o->slaves);

	sqlite3_finalize (o->mark);
	sqlite3_finalize (o->add);
	sqlite3_finalize (o->del_u);
	sqlite3_finalize (o->del_d);
	sqlite3_finalize (o->comm_d);
	sqlite3_finalize (o->comm_u);
	sqlite3_finalize (o->dis_d);
	sqlite3_finalize (o->dis_u);

	sqlite3_close_v2 (o->db);
	free (o);
}

struct yonk_node *yonk_get (struct yonk *o, long id)
{
	const char *req = "SELECT * FROM tree WHERE id = ?";
	struct yonk_node *n = &o->node;

	if (!sqlite_compile (o->db, req, &o->get) ||
	    !sqlite_first (o->get, "l", id))
		return NULL;

	n->parent  = sqlite3_column_int64 (o->get, 1);
	n->link    = sqlite3_column_int64 (o->get, 2);
	n->label   = (void *) sqlite3_column_text  (o->get, 3);
	n->kind    = sqlite3_column_int (o->get, 4);
	n->secure  = sqlite3_column_int (o->get, 5);
	n->active  = sqlite3_column_int (o->get, 6);
	n->dirty   = sqlite3_column_int (o->get, 7);
	n->changed = sqlite3_column_int (o->get, 8);

	return n;
}

long yonk_get_parent (struct yonk *o, long id)
{
	const char *req = "SELECT parent FROM tree WHERE id = ?";

	if (!sqlite_compile (o->db, req, &o->parent) ||
	    !sqlite_first (o->parent, "l", id))
		return 0;

	return sqlite3_column_int64 (o->parent, 1);
}

long yonk_lookup (struct yonk *o, long parent, const char *label)
{
	const char *req = "SELECT id FROM tree WHERE parent = ? AND label = ?";

	if (!sqlite_compile (o->db, req, &o->lookup) ||
	    !sqlite_first (o->lookup, "ls", parent, label))
		return 0;

	return sqlite3_column_int64 (o->lookup, 1);
}

long *yonk_childs (struct yonk *o, long parent, int sorted)
{
	const char *req_n = "SELECT count (id) FROM tree WHERE parent = ?";
	const char *req_u = "SELECT id FROM tree WHERE parent = ?";
	const char *req_s = "SELECT id FROM tree WHERE parent = ?"
			    " ORDER BY label ASC";
	sqlite3_stmt *s;
	long count, *list, i;

	if (!sqlite_compile (o->db, req_n, &o->nchilds) ||
	    !sqlite_first (o->nchilds, "l", parent) ||
	    (count = sqlite3_column_int64 (o->nchilds, 1)) < 0 ||
	    (list = malloc (sizeof (list[0]) * (count + 1))) == NULL)
		return NULL;

	if (!sorted && !sqlite_compile (o->db, req_u, &o->childs))
		goto no_fetch;

	if (sorted && !sqlite_compile (o->db, req_s, &o->childs_s))
		goto no_fetch;

	s = sorted ? o->childs_s : o->childs;

	if (!sqlite_bind (s, "l", parent))
		goto no_fetch;

	for (i = 0; i < count; list[i++] = sqlite3_column_int64 (s, 1))
		if (!sqlite_next (s))
			goto no_fetch;

	list[i] = 0;
	return list;
no_fetch:
	free (list);
	return NULL;
}

long *yonk_slaves (struct yonk *o, long id)
{
	const char *req_n = "SELECT count (id) FROM tree WHERE link = ?";
	const char *req_u = "SELECT id FROM tree WHERE link = ?";
	long count, *list, i;

	if (!sqlite_compile (o->db, req_n, &o->nslaves) ||
	    !sqlite_first (o->nslaves, "l", id) ||
	    (count = sqlite3_column_int64 (o->nslaves, 1)) < 0 ||
	    (list = malloc (sizeof (list[0]) * (count + 1))) == NULL)
		return NULL;

	if (!sqlite_compile (o->db, req_u, &o->slaves) ||
	    !sqlite_bind (o->slaves, "l", id))
		goto no_fetch;

	for (i = 0; i < count; list[i++] = sqlite3_column_int64 (o->slaves, 1))
		if (!sqlite_next (o->slaves))
			goto no_fetch;

	list[i] = 0;
	return list;
no_fetch:
	free (list);
	return NULL;
}

static int yonk_mark (struct yonk *o, long id)
{
	const char *req = "UPDATE tree SET changed = 1 WHERE id = ?";

	if (!sqlite_compile(o->db, req, &o->mark))
		return 0;

	for (; id > 0; id = yonk_get_parent (o, id))
		if (!sqlite_run (o->mark, "l", id))
			return 0;

	return id == 0;
}

long yonk_add (struct yonk *o, long parent, long link, const char *label,
	       int kind, int secure)
{
	const char *req = "INSERT "
			  "INTO tree (parent, link, label, kind, secure) "
			  "VALUES (?, ?, ?, ?, ?)";
	long id;

	if ((id = yonk_lookup (o, parent, label)) != 0)
		return id;

	if (!sqlite_compile (o->db, req, &o->add) ||
	    !sqlite_run (o->add, "llsii", parent, link, label, kind, secure) ||
	    !yonk_mark (o, parent))
		return 0;

	return sqlite3_last_insert_rowid (o->db);
}

static int yonk_delete_tree (struct yonk *o, long id)
{
	const char *req_u = "UPDATE tree SET dirty = 1, changed = 1 "
			    "WHERE id = ? AND active = 1";
	const char *req_d = "DELETE FROM tree WHERE id = ? "
			    "AND active = 0 AND dirty = 1";
	long *list, i;

	if ((list = yonk_childs (o, id, 0)) == NULL)
		return 0;

	for (i = 0; list[i] > 0; ++i)
		if (!yonk_delete_tree (o, list[i]))
			goto no_delete;

	free (list);
	return 	sqlite_compile (o->db, req_u, &o->del_u) &&
		sqlite_run (o->del_u, "l", id) &&
		sqlite_compile (o->db, req_d, &o->del_d) &&
		sqlite_run (o->del_d, "l", id);
no_delete:
	free (list);
	return 0;
}

int yonk_delete (struct yonk *o, long id)
{
	long *list, i;
	
	if ((list = yonk_slaves (o, id)) == NULL)
		return 0;

	for (i = 0; list[i] > 0; ++i)
		if (!yonk_delete (o, list[i]))
			goto no_delete;

	free (list);
	return yonk_delete_tree (o, id) && yonk_mark (o, id);
no_delete:
	free (list);
	return 0;
}

int yonk_commit (struct yonk *o)
{
	const char *req_d = "DELETE FROM tree WHERE active = 1 AND dirty = 1";
	const char *req_u = "UPDATE tree SET active = 1, dirty = 0, changed = 0";

	return 	sqlite_compile (o->db, req_d, &o->comm_d) &&
		sqlite_run (o->comm_d, "") &&
		sqlite_compile (o->db, req_u, &o->comm_u) &&
		sqlite_run (o->comm_u, "");
}

int yonk_discard (struct yonk *o)
{
	const char *req_d = "DELETE FROM tree WHERE active = 0 AND dirty = 1";
	const char *req_u = "UPDATE tree SET dirty = 0, changed = 0";

	return	sqlite_compile (o->db, req_d, &o->dis_d) &&
		sqlite_run (o->dis_d, "") &&
		sqlite_compile (o->db, req_u, &o->dis_u) &&
		sqlite_run (o->dis_u, "");
}
