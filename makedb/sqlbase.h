
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

