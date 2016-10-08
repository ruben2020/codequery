
/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include "sqlbase.h"

sqlbase::sqlbase()
:m_db(NULL)
,m_debug(false)
{
}

sqlbase::~sqlbase()
{
	sqlite3_close(m_db);
}

void sqlbase::setDebug(bool val)
{
	m_debug = val;
}

int sqlbase::vacuum(const char* fn, const bool& debug)
{
	int rc;
	sqlite3 *sqdb;
	rc = sqlite3_open_v2(fn, &sqdb, SQLITE_OPEN_READWRITE, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_close(sqdb);
		return rc;
	}
	rc=sqlite3_exec(sqdb, "PRAGMA synchronous = OFF;"
		"PRAGMA journal_mode = OFF;"
		"PRAGMA locking_mode = EXCLUSIVE;"
		"PRAGMA automatic_index = FALSE;"
		"PRAGMA cache_size = 20000;"
		"VACUUM;ANALYZE;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (debug) printf("SQLBaseErr099: %d, %s\n", rc, sqlite3_errmsg(sqdb));
	}
	sqlite3_close(sqdb);
	return rc;
}

int sqlbase::analyze(const char* fn, const bool& debug)
{
	int rc;
	sqlite3 *sqdb;
	rc = sqlite3_open_v2(fn, &sqdb, SQLITE_OPEN_READWRITE, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_close(sqdb);
		return rc;
	}
	rc=sqlite3_exec(sqdb, "PRAGMA synchronous = OFF;"
		"PRAGMA journal_mode = OFF;"
		"PRAGMA locking_mode = EXCLUSIVE;"
		"PRAGMA automatic_index = FALSE;"
		"PRAGMA cache_size = 20000;"
		"ANALYZE;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (debug) printf("SQLBaseErr100: %d, %s\n", rc, sqlite3_errmsg(sqdb));
	}
	sqlite3_close(sqdb);
	return rc;
}

int sqlbase::prepare_stmt(sqlite3_stmt** pStmt, const char* sqlquery)
{
	int rc;
	rc = sqlite3_prepare_v2(m_db, sqlquery, strlen(sqlquery), pStmt, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr001: %d, %s, line=%s\n", rc, sqlite3_errmsg(m_db), sqlquery);
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int sqlbase::execstmt(sqlite3_stmt* pstmt, const char* v1)
{
	int rc;
	rc = sqlite3_reset(pstmt);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr004: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 1, v1, strlen(v1), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr002: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_step(pstmt);
	if ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY)) return rc;
	if (rc != SQLITE_DONE)
	{
		if (m_debug) printf("SQLBaseErr003: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int sqlbase::execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2)
{
	int rc;
	rc = sqlite3_reset(pstmt);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr004: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 1, v1, strlen(v1), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr005: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 2, v2, strlen(v2), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr006: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_step(pstmt);
	if ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY)) return rc;
	if (rc != SQLITE_DONE)
	{
		if (m_debug) printf("SQLBaseErr007: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int sqlbase::execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2, const char* v3)
{
	int rc;
	rc = sqlite3_reset(pstmt);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr004: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 1, v1, strlen(v1), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr009: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 2, v2, strlen(v2), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr010: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 3, v3, strlen(v3), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr011: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_step(pstmt);
	if ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY)) return rc;
	if (rc != SQLITE_DONE)
	{
		if (m_debug) printf("SQLBaseErr012: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int sqlbase::execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2, const char* v3, const char* v4)
{
	int rc;
	rc = sqlite3_reset(pstmt);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr004: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 1, v1, strlen(v1), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr014: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 2, v2, strlen(v2), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr015: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 3, v3, strlen(v3), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr016: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 4, v4, strlen(v4), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr017: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_step(pstmt);
	if ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY)) return rc;
	if (rc != SQLITE_DONE)
	{
		if (m_debug) printf("SQLBaseErr018: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int sqlbase::execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2, const char* v3, const char* v4, const char* v5)
{
	int rc;
	rc = sqlite3_reset(pstmt);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr004: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 1, v1, strlen(v1), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr020: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 2, v2, strlen(v2), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr021: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 3, v3, strlen(v3), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr022: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 4, v4, strlen(v4), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr023: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_bind_text(pstmt, 5, v5, strlen(v5), SQLITE_STATIC);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLBaseErr024: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	rc = sqlite3_step(pstmt);
	if ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY)) return rc;
	if (rc != SQLITE_DONE)
	{
		if (m_debug) printf("SQLBaseErr025: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

