
// Library to query CodeQuery database files
//
// This library is MIT-licensed, so that it may be used to create plugins
// for editors, IDEs and other software without license restrictions
//
// This license applies only to this file:
//
// Copyright (c) 2011 ruben2020 https://github.com/ruben2020/
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <sqlite3.h>
#include "small_lib.h"
#include "sqlquery.h"

#ifdef _WIN32
#define DIRSEP '\\'
#else
#define DIRSEP '/'
#endif


#define SQL_SYM "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_FUNC_MACRO "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\") AND (symtbl.symType=\"$\" OR symtbl.symType=\"#\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_CLASS_STRUCT "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\") AND (symtbl.symType=\"c\" OR symtbl.symType=\"s\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_CALLINGFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT callerID FROM calltbl WHERE calledID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_CALLEDFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT calledID FROM calltbl WHERE callerID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_MEMBERS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT memberID FROM membertbl WHERE groupID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_OWNERCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT groupID FROM membertbl WHERE memberID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_PARENTCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT parentID FROM inherittbl WHERE childID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_CHILDCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT childID FROM inherittbl WHERE parentID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_INCLUDE "SELECT filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\" AND symType=\"~\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_FILEPATH "SELECT filePath FROM filestbl WHERE filePath LIKE ? ESCAPE \";\";"
#define SQL_AUTOCOMPLETE "SELECT DISTINCT symName FROM symtbl WHERE symName LIKE ? ESCAPE \";\" ORDER BY symName;"

#define SQL_EM_SYM "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_FUNC_MACRO "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) AND (symtbl.symType=\"$\" OR symtbl.symType=\"#\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CLASS_STRUCT "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) AND (symtbl.symType=\"c\" OR symtbl.symType=\"s\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CALLINGFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT callerID FROM calltbl WHERE calledID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CALLEDFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT calledID FROM calltbl WHERE callerID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_MEMBERS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT memberID FROM membertbl WHERE groupID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_OWNERCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT groupID FROM membertbl WHERE memberID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_PARENTCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT parentID FROM inherittbl WHERE childID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CHILDCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT childID FROM inherittbl WHERE parentID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_INCLUDE "SELECT filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=? AND symType=\"~\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_FILEPATH "SELECT filePath FROM filestbl WHERE filePath=?;"


tempstmt::tempstmt()
:m_stmt(NULL)
{
}

tempstmt::~tempstmt()
{
	finalize();
}

void tempstmt::finalize(void)
{
	sqlite3_finalize(m_stmt);
	m_stmt = NULL;
}

sqlite3_stmt* tempstmt::get(void)
{
	return m_stmt;
}

sqlqueryresultlist::sqlqueryresultlist()
:result_type(sqlresultERROR)
,sqlerrmsg("empty")
{
}

sqlqueryresultlist::sqlqueryresultlist(const sqlqueryresultlist& copy)
{
	result_type = copy.result_type;
	sqlerrmsg = copy.sqlerrmsg;
	resultlist = copy.resultlist;
}

sqlqueryresultlist& sqlqueryresultlist::operator= (const sqlqueryresultlist& copy)
{
	if (&copy != this)
	{
		result_type = copy.result_type;
		sqlerrmsg = copy.sqlerrmsg;
		resultlist.clear();
		resultlist = copy.resultlist;
	}
	return *this;
}

sqlquery::sqlquery()
:m_db(NULL)
{
}

sqlquery::~sqlquery()
{
	close_dbfile();
}


sqlquery::en_filereadstatus sqlquery::open_dbfile(tStr dbfn)
{
	if (dbfn.empty()) return sqlfileOPENERROR;

	smartFILE fp;
	// Does the file exist?
	if (access(dbfn.c_str(), F_OK) == -1) {return sqlfileOPENERROR;}
	// Try to open the file for reading
	fp = fopen(dbfn.c_str(), "r");
	if (fp == NULL) {return sqlfileOPENERROR;}
	fp.close_file();

	int rc = sqlite3_open_v2(dbfn.c_str(),
						&m_db, SQLITE_OPEN_READONLY, NULL);
	if ((rc != SQLITE_OK)||(m_db == NULL)) 
	{
		close_dbfile();
		return sqlfileOPENERROR;
	}
	tempstmt stmt;
	tStr majorver = read_configtbl("DB_MAJOR_VER", stmt.get());
	tStr minorver = read_configtbl("DB_MINOR_VER", stmt.get());
	if ((majorver.empty())||(minorver.empty()))
		{return sqlfileNOTCORRECTDB;}
	if (majorver.compare(tStr("0")) != 0) return sqlfileINCORRECTVER;
	if (minorver.compare(tStr("1")) != 0) return sqlfileINCORRECTVER;
	m_basepath = read_configtbl("DB_BASE_PATH", stmt.get());
	if (m_basepath.empty()) {return sqlfileNOTCORRECTDB;}
	rc = sqlite3_prepare_v2(m_db, SQL_AUTOCOMPLETE, strlen(SQL_AUTOCOMPLETE),
							&(m_autocompstmt.m_stmt), NULL);
	if (rc != SQLITE_OK) {return sqlfileNOTCORRECTDB;}
	return sqlfileOK;
}

void sqlquery::close_dbfile(void)
{
	m_autocompstmt.finalize();
	sqlite3_close(m_db);
	m_db = NULL;
	m_basepath.clear();
}

tStr sqlquery::read_configtbl(const char *key, sqlite3_stmt *stmt)
{
	tStr result = "";
	if ((key == NULL)||(strlen(key) == 0)||(m_db == NULL)) return result;
	int rc;
	if (stmt == NULL)
	{
		rc = sqlite3_prepare_v2(m_db, "SELECT configVal FROM configtbl WHERE configKey=?;",
							strlen("SELECT configVal FROM configtbl WHERE configKey=?;"),
							&stmt, NULL);
		if (rc != SQLITE_OK) {return result;}
	}
	else sqlite3_reset(stmt);
	rc = sqlite3_bind_text(stmt, 1, key, strlen(key), SQLITE_STATIC);
	if (rc != SQLITE_OK) {return result;}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {return result;}
	result = (const char*) sqlite3_column_text(stmt, 0);
	return result;
}

tVecStr sqlquery::search_autocomplete(const char* searchstr)
{
	tVecStr result;
	int ctr = 0;
	if ((searchstr == NULL)||(strlen(searchstr) < 1)) return result;
	tStr srchterm = process_searchterm_autocomplete(searchstr);
	sqlite3_reset(m_autocompstmt.get());
	int rc = sqlite3_bind_text(m_autocompstmt.get(), 1, srchterm.c_str(), srchterm.size(), SQLITE_STATIC);
	if (rc != SQLITE_OK) {printf("Err: %s\n", sqlite3_errmsg(m_db)); return result;}
	do
	{
		rc = sqlite3_step(m_autocompstmt.get());
		if (rc == SQLITE_ROW)
		{
			result.push_back(tStr((const char*) sqlite3_column_text(m_autocompstmt.get(), 0)));
			if (ctr++ > 300) rc = SQLITE_DONE;
		}
	} while (rc == SQLITE_ROW);
	if (rc != SQLITE_DONE)
	{
		printf("Err: %s\n", sqlite3_errmsg(m_db));
	}
	return result;
}


sqlqueryresultlist sqlquery::search(
						tStr searchstr,
						sqlquery::en_queryType querytype,
						bool exactmatch)
{
	sqlqueryresultlist result;
	int rc;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	if ((m_db == NULL)||(searchstr.empty())||(m_basepath.empty())) return result;
	tStr sqlqry, srchterm;
	sqlqueryresultlist::en_resultType resultType = sqlqueryresultlist::sqlresultFULL;
	srchterm = process_searchterm(searchstr.c_str(), exactmatch);
	switch (querytype)
	{
		case sqlquerySYMBOL:
			sqlqry = exactmatch ? SQL_EM_SYM : SQL_SYM;
			break;
		case sqlresultFUNC_MACRO:
			sqlqry = exactmatch ? SQL_EM_FUNC_MACRO : SQL_FUNC_MACRO;
			break;
		case sqlresultCLASS_STRUCT:
			sqlqry = exactmatch ? SQL_EM_CLASS_STRUCT : SQL_CLASS_STRUCT;
			break;
		case sqlresultINCLUDE:
			sqlqry = exactmatch ? SQL_EM_INCLUDE : SQL_INCLUDE;
			resultType = sqlqueryresultlist::sqlresultFILE_LINE;
			break;
		case sqlresultFILEPATH:
			sqlqry = exactmatch ? SQL_EM_FILEPATH : SQL_FILEPATH;
			resultType = sqlqueryresultlist::sqlresultFILE_ONLY;
			break;
		case sqlresultCALLINGFUNC:
			sqlqry = exactmatch ? SQL_EM_CALLINGFUNC : SQL_CALLINGFUNC;
			break;
		case sqlresultCALLEDFUNC:
			sqlqry = exactmatch ? SQL_EM_CALLEDFUNC : SQL_CALLEDFUNC;
			break;
		case sqlresultMEMBERS:
			sqlqry = exactmatch ? SQL_EM_MEMBERS : SQL_MEMBERS;
			break;
		case sqlresultOWNERCLASS:
			sqlqry = exactmatch ? SQL_EM_OWNERCLASS : SQL_OWNERCLASS;
			break;
		case sqlresultPARENTCLASS:
			sqlqry = exactmatch ? SQL_EM_PARENTCLASS : SQL_PARENTCLASS;
			break;
		case sqlresultCHILDCLASS:
			sqlqry = exactmatch ? SQL_EM_CHILDCLASS : SQL_CHILDCLASS;
			break;
		case sqlresultAUTOCOMPLETE:
			resultType = sqlqueryresultlist::sqlresultSYM_ONLY;
		default:
			result.sqlerrmsg = "Unknown search type";
			return result;
	}
	tempstmt stmt;
	rc = sqlite3_prepare_v2(m_db, sqlqry.c_str(),
									sqlqry.size(),
									&(stmt.m_stmt), NULL);
	if (rc != SQLITE_OK) {result.sqlerrmsg = sqlite3_errmsg(m_db); return result;}
	rc = sqlite3_bind_text(stmt.get(), 1, srchterm.c_str(), srchterm.size(), SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {result.sqlerrmsg = sqlite3_errmsg(m_db); return result;}
	if (resultType == sqlqueryresultlist::sqlresultFULL) result = search_full(stmt.get());
	else if (resultType == sqlqueryresultlist::sqlresultFILE_LINE) result = search_file_line(stmt.get());
	else if (resultType == sqlqueryresultlist::sqlresultFILE_ONLY) result = search_file_only(stmt.get());
	return result;
}	

tStr sqlquery::process_searchterm(const char* searchterm, const bool& exactmatch)
{
	tStr srchterm, srchterm2;
	if (!exactmatch)
	{
		srchterm2 = add_escape_char(searchterm,        '%', ';').c_str();
		srchterm2 = add_escape_char( srchterm2.c_str(), '_', ';').c_str();
		srchterm = "%";
		srchterm += srchterm2;
		srchterm += "%";
		std::replace( srchterm.begin(), srchterm.end(), '*', '%');
		std::replace( srchterm.begin(), srchterm.end(), '?', '_');
	}
	else srchterm = searchterm;
	return srchterm;
}

tStr sqlquery::process_searchterm_autocomplete(const char* searchterm)
{
	tStr srchterm;//, srchterm2="%";
	srchterm = add_escape_char(searchterm,         '%', ';').c_str();
	srchterm = add_escape_char(  srchterm.c_str(), '_', ';').c_str();
	srchterm += "%";
	//srchterm2 += srchterm;
	return srchterm;//2;
}


sqlqueryresultlist sqlquery::search_full(sqlite3_stmt* stmt)
{
	int rc;
	sqlqueryresultlist result;
	tStr fp;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	do
	{
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW)
		{
			sqlqueryresult item;
			item.symname  = (const char*) sqlite3_column_text(stmt, 0);
			item.symtype  = (const char*) sqlite3_column_text(stmt, 1);
			fp            = (const char*) sqlite3_column_text(stmt, 2);
			item.linenum  = (const char*) sqlite3_column_text(stmt, 3);
			item.linetext = (const char*) sqlite3_column_text(stmt, 4);
			item.filename = extract_filename(fp.c_str());
			if (fp[0] != (char)DIRSEP)
			{
				item.filepath = m_basepath;
				item.filepath += DIRSEP;
				item.filepath += fp;
			}
			else item.filepath = fp;
			result.resultlist.push_back(item);
		}
	} while (rc == SQLITE_ROW);
	if (rc != SQLITE_DONE)
	{
		result.result_type = sqlqueryresultlist::sqlresultERROR;
		result.sqlerrmsg = sqlite3_errmsg(m_db);
	}
	else
	{
		result.result_type = sqlqueryresultlist::sqlresultFULL;
	}
	return result;
}

sqlqueryresultlist sqlquery::search_file_line(sqlite3_stmt* stmt)
{
	int rc;
	sqlqueryresultlist result;
	tStr fp;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	do
	{
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW)
		{
			sqlqueryresult item;
			fp            = (const char*) sqlite3_column_text(stmt, 0);
			item.linenum  = (const char*) sqlite3_column_text(stmt, 1);
			item.linetext = (const char*) sqlite3_column_text(stmt, 2);
			item.filename = extract_filename(fp.c_str());
			if (fp[0] != (char)DIRSEP)
			{
				item.filepath = m_basepath;
				item.filepath += DIRSEP;
				item.filepath += fp;
			}
			else item.filepath = fp;
			result.resultlist.push_back(item);
		}
	} while (rc == SQLITE_ROW);
	if (rc != SQLITE_DONE)
	{
		result.result_type = sqlqueryresultlist::sqlresultERROR;
		result.sqlerrmsg = sqlite3_errmsg(m_db);
	}
	else
	{
		result.result_type = sqlqueryresultlist::sqlresultFILE_LINE;
	}
	return result;
}

sqlqueryresultlist sqlquery::search_file_only(sqlite3_stmt* stmt)
{
	int rc;
	sqlqueryresultlist result;
	tStr fp;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	do
	{
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW)
		{
			sqlqueryresult item;
			fp            = (const char*) sqlite3_column_text(stmt, 0);
			item.linenum  = "1";
			item.filename = extract_filename(fp.c_str());
			if (fp[0] != (char)DIRSEP)
			{
				item.filepath = m_basepath;
				item.filepath += DIRSEP;
				item.filepath += fp;
			}
			else item.filepath = fp;
			result.resultlist.push_back(item);
		}
	} while (rc == SQLITE_ROW);
	if (rc != SQLITE_DONE)
	{
		result.result_type = sqlqueryresultlist::sqlresultERROR;
		result.sqlerrmsg = sqlite3_errmsg(m_db);
	}
	else
	{
		result.result_type = sqlqueryresultlist::sqlresultFILE_ONLY;
	}
	return result;
}




