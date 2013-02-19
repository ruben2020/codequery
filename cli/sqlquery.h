
/*
 * CodeQuery
 * Copyright (C) 2013 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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


#ifndef _SQLQUERY_H
#define _SQLQUERY_H

#include "small_lib.h"

// forward declaration
class sqlite3;
class sqlite3_stmt;

class tempstmt
{
public:
sqlite3_stmt *m_stmt;
tempstmt();
~tempstmt();
void finalize(void);
sqlite3_stmt* get(void);
};

class sqlqueryresult
{
public:
	tStr symname;
	tStr symtype;
	tStr linenum;
	tStr filename;
	tStr filepath;
	tStr linetext;
	sqlqueryresult() : linenum((const char*)"1"){}
};

class sqlqueryresultlist
{
public:
enum en_resultType
	{
		sqlresultFULL = 0,
		sqlresultFILE_LINE,
		sqlresultFILE_ONLY,
		sqlresultSYM_ONLY,
		sqlresultERROR
	};
	en_resultType result_type;
	std::vector<sqlqueryresult> resultlist;
	sqlqueryresultlist();
	sqlqueryresultlist(const sqlqueryresultlist& copy);
	sqlqueryresultlist& operator= (const sqlqueryresultlist& copy);
	tStr sqlerrmsg;
};


class sqlquery
{
public:
enum en_queryType
	{
		sqlquerySYMBOL = 0,
		sqlresultFUNC_MACRO,
		sqlresultCLASS_STRUCT,
		sqlresultINCLUDE,
		sqlresultFILEPATH,
		sqlresultCALLINGFUNC,
		sqlresultCALLEDFUNC,
		sqlresultMEMBERS,
		sqlresultOWNERCLASS,
		sqlresultCHILDCLASS,
		sqlresultPARENTCLASS,
		sqlresultAUTOCOMPLETE,
		sqlresultDEFAULT
	};
enum en_filereadstatus
	{
		sqlfileOK = 0,
		sqlfileOPENERROR,
		sqlfileNOTCORRECTDB,
		sqlfileINCORRECTVER,
		sqlfileUNKNOWNERROR
	};	
	sqlquery();
	~sqlquery();
	bool isDBOpen(void) {return (m_db != NULL);}
	en_filereadstatus open_dbfile(tStr dbfn);
	void close_dbfile(void);
	sqlqueryresultlist search(tStr searchstr,
				en_queryType querytype = sqlquerySYMBOL,
				bool exactmatch=false);
	tVecStr search_autocomplete(const char* searchstr);

private:
	sqlite3 *m_db;
	tStr m_basepath;
	tempstmt m_autocompstmt;
	sqlqueryresultlist search_full(sqlite3_stmt* stmt);
	sqlqueryresultlist search_file_line(sqlite3_stmt* stmt);
	sqlqueryresultlist search_file_only(sqlite3_stmt* stmt);
	tStr read_configtbl(const char *key, sqlite3_stmt *stmt);
	tStr process_searchterm(const char* searchterm, const bool& exactmatch);
	tStr process_searchterm_autocomplete(const char* searchstr);
};

#endif
