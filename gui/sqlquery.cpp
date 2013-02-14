
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
#define SQL_PARENTCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT groupID FROM membertbl WHERE memberID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\")) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_INCLUDE "SELECT filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName LIKE ? ESCAPE \";\" AND symType=\"~\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_FILEPATH "SELECT filePath FROM filestbl WHERE filePath LIKE ? ESCAPE \";\";"
#define SQL_AUTOCOMPLETE "SELECT DISTINCT symName FROM symtbl WHERE symName LIKE ? ESCAPE \";\" ORDER BY symName;"

#define SQL_EM_SYM "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_FUNC_MACRO "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) AND (symtbl.symType=\"$\" OR symtbl.symType=\"#\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CLASS_STRUCT "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT symID FROM symtbl WHERE symName=?) AND (symtbl.symType=\"c\" OR symtbl.symType=\"s\") INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CALLINGFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT callerID FROM calltbl WHERE calledID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_CALLEDFUNC "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT calledID FROM calltbl WHERE callerID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_MEMBERS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT memberID FROM membertbl WHERE groupID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
#define SQL_EM_PARENTCLASS "SELECT symtbl.symName,symtbl.symType,filestbl.filePath,linestbl.linenum,linestbl.linetext FROM symtbl INNER JOIN linestbl ON symtbl.lineID=linestbl.lineID AND symtbl.symID IN (SELECT groupID FROM membertbl WHERE memberID IN (SELECT symID FROM symtbl WHERE symName=?)) INNER JOIN filestbl ON linestbl.fileID=filestbl.fileID;"
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


sqlquery::en_filereadstatus sqlquery::open_dbfile(QString dbfn)
{
	if (dbfn.isEmpty()) return sqlfileOPENERROR;
	QFile file(dbfn);
	if(!file.exists()) return sqlfileOPENERROR;
	if(!file.open(QIODevice::ReadOnly))  {return sqlfileOPENERROR;}
	file.close();
	int rc = sqlite3_open_v2(dbfn.toAscii().data(),
						&m_db, SQLITE_OPEN_READONLY, NULL);
	if ((rc != SQLITE_OK)||(m_db == NULL)) 
	{
		close_dbfile();
		return sqlfileOPENERROR;
	}
	tempstmt stmt;
	QString majorver = read_configtbl("DB_MAJOR_VER", stmt.get());
	QString minorver = read_configtbl("DB_MINOR_VER", stmt.get());
	if ((majorver.isEmpty())||(minorver.isEmpty()))
		{return sqlfileNOTCORRECTDB;}
	if (majorver.compare(QString("0")) != 0) return sqlfileINCORRECTVER;
	if (minorver.compare(QString("1")) != 0) return sqlfileINCORRECTVER;
	m_basepath = read_configtbl("DB_BASE_PATH", stmt.get());
	if (m_basepath.isEmpty()) {return sqlfileNOTCORRECTDB;}
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

QString sqlquery::read_configtbl(const char *key, sqlite3_stmt *stmt)
{
	QString result = "";
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

QStringList sqlquery::search_autocomplete(const QString& searchstr)
{
	QStringList result;
	int ctr = 0;
	if (searchstr.toAscii().size() < 1) return result;
	QString srchterm = process_searchterm_autocomplete(searchstr);
	sqlite3_reset(m_autocompstmt.get());
	int rc = sqlite3_bind_text(m_autocompstmt.get(), 1, srchterm.toAscii().data(), srchterm.toAscii().size(), SQLITE_STATIC);
	if (rc != SQLITE_OK) {printf("Err: %s\n", sqlite3_errmsg(m_db)); return result;}
	do
	{
		rc = sqlite3_step(m_autocompstmt.get());
		if (rc == SQLITE_ROW)
		{
			result.push_back(QString((const char*) sqlite3_column_text(m_autocompstmt.get(), 0)));
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
						QString searchstr,
						sqlquery::en_queryType querytype,
						bool exactmatch)
{
	sqlqueryresultlist result;
	int rc;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	if ((m_db == NULL)||(searchstr.isEmpty())||(m_basepath.isEmpty())) return result;
	QString sqlqry, srchterm;
	sqlqueryresultlist::en_resultType resultType = sqlqueryresultlist::sqlresultFULL;
	srchterm = process_searchterm(searchstr, exactmatch);
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
		case sqlresultPARENTCLASS:
			sqlqry = exactmatch ? SQL_EM_PARENTCLASS : SQL_PARENTCLASS;
			break;
		case sqlresultAUTOCOMPLETE:
			resultType = sqlqueryresultlist::sqlresultSYM_ONLY;
		default:
			result.sqlerrmsg = "Unknown search type";
			return result;
	}
	tempstmt stmt;
	rc = sqlite3_prepare_v2(m_db, sqlqry.toAscii().data(),
									sqlqry.toAscii().size(),
									&(stmt.m_stmt), NULL);
	if (rc != SQLITE_OK) {result.sqlerrmsg = sqlite3_errmsg(m_db); return result;}
	rc = sqlite3_bind_text(stmt.get(), 1, srchterm.toAscii().data(), srchterm.toAscii().size(), SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {result.sqlerrmsg = sqlite3_errmsg(m_db); return result;}
	if (resultType == sqlqueryresultlist::sqlresultFULL) result = search_full(stmt.get());
	else if (resultType == sqlqueryresultlist::sqlresultFILE_LINE) result = search_file_line(stmt.get());
	else if (resultType == sqlqueryresultlist::sqlresultFILE_ONLY) result = search_file_only(stmt.get());
	return result;
}	

QString sqlquery::process_searchterm(const QString& searchterm, const bool& exactmatch)
{
	QString srchterm, srchterm2;
	if (!exactmatch)
	{
		srchterm2 = add_escape_char(searchterm.toAscii().data(), '%', ';').c_str();
		srchterm2 = add_escape_char( srchterm2.toAscii().data(), '_', ';').c_str();
		srchterm = "%";
		srchterm += srchterm2;
		srchterm += "%";
	}
	else srchterm = searchterm;
	return srchterm;
}

QString sqlquery::process_searchterm_autocomplete(const QString& searchterm)
{
	QString srchterm;//, srchterm2="%";
	srchterm = add_escape_char(searchterm.toAscii().data(), '%', ';').c_str();
	srchterm = add_escape_char(  srchterm.toAscii().data(), '_', ';').c_str();
	srchterm += "%";
	//srchterm2 += srchterm;
	return srchterm;//2;
}


sqlqueryresultlist sqlquery::search_full(sqlite3_stmt* stmt)
{
	int rc;
	sqlqueryresultlist result;
	QString fp;
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
			item.filename = extract_filename(fp.toAscii().data());
			if (fp[0] != QChar(DIRSEP))
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
	QString fp;
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
			item.filename = extract_filename(fp.toAscii().data());
			if (fp[0] != QChar(DIRSEP))
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
	QString fp;
	result.result_type = sqlqueryresultlist::sqlresultERROR;
	do
	{
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW)
		{
			sqlqueryresult item;
			fp            = (const char*) sqlite3_column_text(stmt, 0);
			item.linenum  = "1";
			item.filename = extract_filename(fp.toAscii().data());
			if (fp[0] != QChar(DIRSEP))
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

const char* sqlquery::errormsg(sqlquery::en_filereadstatus status)
{
	switch(status)
	{
		case sqlfileOPENERROR: return "File open error";
		case sqlfileNOTCORRECTDB: return "Wrong file format";
		case sqlfileINCORRECTVER: return "Incorrect DB Version";
		case sqlfileOK: return "OK";
		case sqlfileUNKNOWNERROR:
		default: return "Unknown Error";
	}
}



