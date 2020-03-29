
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef SQLBASE_H_CQ
#define SQLBASE_H_CQ

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>


class sqlbase
{

public:
sqlbase();
~sqlbase();
void setDebug(bool val);
static int vacuum(const char* fn, const bool& debug);
static int analyze(const char* fn, const bool& debug);
int prepare_stmt(sqlite3_stmt** pStmt, const char* sqlquery);
int execstmt(sqlite3_stmt* pstmt);
int execstmt(sqlite3_stmt* pstmt, const char* v1);
int execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2);
int execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2,
									const char* v3);
int execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2,
							const char* v3, const char* v4);
int execstmt(sqlite3_stmt* pstmt, const char* v1, const char* v2,
					const char* v3, const char* v4, const char* v5);

protected:
bool m_debug;
sqlite3 *m_db;

};

#endif

