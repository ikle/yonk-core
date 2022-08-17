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
	sqlite3_stmt *get, *parent, *lookup_w, *lookup_n;
	sqlite3_stmt *nchilds, *childs, *childs_s, *nslaves, *slaves;
	sqlite3_stmt *mark, *add, *del_u, *del_d;
	sqlite3_stmt *comm_d, *comm_u, *dis_d, *dis_u;
};

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

	o->get      = NULL;
	o->parent   = NULL;
	o->lookup_w = NULL;
	o->lookup_n = NULL;
	o->nchilds  = NULL;
	o->childs   = NULL;
	o->childs_s = NULL;
	o->nslaves  = NULL;
	o->slaves   = NULL;

	o->mark     = NULL;
	o->add      = NULL;
	o->del_u    = NULL;
	o->del_d    = NULL;
	o->comm_d   = NULL;
	o->comm_u   = NULL;
	o->dis_d    = NULL;
	o->dis_u    = NULL;

	return o;
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
	sqlite3_finalize (o->lookup_w);
	sqlite3_finalize (o->lookup_n);
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
	    !sqlite_bind (o->get, "l", id) ||
	    sqlite3_step (o->get) != SQLITE_ROW)
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
	    !sqlite_bind (o->parent, "l", id) ||
	    sqlite3_step (o->parent) != SQLITE_ROW)
		return 0;

	return sqlite3_column_int64 (o->parent, 1);
}

long yonk_lookup (struct yonk *o, long parent, const char *label, int active)
{
	const char *req_w = "SELECT id FROM tree WHERE parent = ? AND label = ?"
			    " AND active = 1";
	const char *req_n = "SELECT id FROM tree WHERE parent = ? AND label = ?"
			    " AND NOT (active = 1 AND dirty = 1)";
	sqlite3_stmt *s;

	if (active && !sqlite_compile (o->db, req_w, &o->lookup_w))
		return 0;

	if (!active && !sqlite_compile (o->db, req_n, &o->lookup_n))
		return 0;

	s = active ? o->lookup_w : o->lookup_n;

	if (!sqlite_bind (s, "ls", parent, label) ||
	    sqlite3_step (s) != SQLITE_ROW)
		return 0;

	return sqlite3_column_int64 (s, 1);
}

long *yonk_childs (struct yonk *o, long parent, int sorted)
{
	const char *req_n = "SELECT count (id) FROM tree WHERE parent = ?";
	const char *req_u = "SELECT id FROM tree WHERE parent = ?";
	const char *req_s = "SELECT id FROM tree WHERE parent = ?"
			    " ORDER BY label ASC";
	sqlite3_stmt *s;
	long count, *list, i;

	if (!sqlite_compile (o->db, req_n, &o->nchilds))
		return NULL;

	s = o->nchilds;

	if (!sqlite_bind (s, "l", parent) ||
	    sqlite3_step (s) != SQLITE_ROW)
		return NULL;

	count = sqlite3_column_int64 (s, 1);

	if (count < 0 ||
	    (list = malloc (sizeof (list[0]) * (count + 1))) == NULL)
		return NULL;

	if (!sorted && !sqlite_compile (o->db, req_u, &o->childs))
		return NULL;

	if (sorted && !sqlite_compile (o->db, req_s, &o->childs_s))
		return NULL;

	s = sorted ? o->childs_s : o->childs;

	if (!sqlite_bind (s, "l", parent))
		return NULL;

	for (i = 0; i < count; ++i)
		if (sqlite3_step (s) != SQLITE_ROW)
			goto no_fetch;
		else
			list[i] = sqlite3_column_int64 (s, 1);

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
	sqlite3_stmt *s;
	long count, *list, i;

	if (!sqlite_compile (o->db, req_n, &o->nslaves))
		return NULL;

	s = o->nslaves;

	if (!sqlite_bind (s, "l", id) ||
	    sqlite3_step (s) != SQLITE_ROW)
		return NULL;

	count = sqlite3_column_int64 (s, 1);

	if (count < 0 ||
	    (list = malloc (sizeof (list[0]) * (count + 1))) == NULL)
		return NULL;

	if (!sqlite_compile (o->db, req_u, &o->slaves))
		return NULL;

	s = o->slaves;

	if (!sqlite_bind (s, "l", id))
		return NULL;

	for (i = 0; i < count; ++i)
		if (sqlite3_step (s) != SQLITE_ROW)
			goto no_fetch;
		else
			list[i] = sqlite3_column_int64 (s, 1);

	list[i] = 0;
	return list;
no_fetch:
	free (list);
	return NULL;
}

/* modify */

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
	const char *req = "INSERT OR REPLACE "
			  "INTO tree (parent, link, label, kind, secure) "
			  "VALUES (?, ?, ?, ?, ?)";

	if (!sqlite_compile (o->db, req, &o->add) ||
	    !sqlite_run (o->add, "llsii", parent, link, label, kind, secure))
		return 0;

	if (!yonk_mark (o, parent))
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

	if (!sqlite_compile (o->db, req_u, &o->del_u) ||
	    !sqlite_run (o->del_u, "l", id))
		return 0;

	if (!sqlite_compile (o->db, req_d, &o->del_d) ||
	    !sqlite_run (o->del_d, "l", id))
		return 0;

	return 1;
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

	if (!sqlite_compile (o->db, req_d, &o->comm_d) ||
	    !sqlite_run (o->comm_d, ""))
		return 0;

	if (!sqlite_compile (o->db, req_u, &o->comm_u) ||
	    !sqlite_run (o->comm_u, ""))
		return 0;

	return 1;
}

int yonk_discard (struct yonk *o)
{
	const char *req_d = "DELETE FROM tree WHERE active = 0 AND dirty = 1";
	const char *req_u = "UPDATE tree SET dirty = 0, changed = 0";

	if (!sqlite_compile (o->db, req_d, &o->dis_d) ||
	    !sqlite_run (o->dis_d, ""))
		return 0;

	if (!sqlite_compile (o->db, req_u, &o->dis_u) ||
	    !sqlite_run (o->dis_u, ""))
		return 0;

	return 1;
}
