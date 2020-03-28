
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include "cs2sq.h"
#include "small_lib.h"

cs2sq::cs2sq()
:m_csdbpLastErr(csdbparser::resOK)
,m_buf(NULL)
,m_filesstmt(NULL)
,m_linesstmt(NULL)
,m_callstmt(NULL)
,m_symstmt(NULL)
{
	m_buf = new char[1000];
}

cs2sq::~cs2sq()
{
	close_csdb();
	close_db();
	delete[] m_buf;
}

csdbparser::enResult cs2sq::open_csdb(const char* csdbfn)
{
	return m_csdbp.open_file(csdbfn);
}

void cs2sq::close_csdb(void)
{
	m_csdbp.close_file();
}

csdbparser::enResult cs2sq::test_csdb(void)
{
	csdbparser::enResult res;
	if (isCSDBFileOpen() == false) return csdbparser::resFILE_NOT_OPEN;
	
	if (m_debug)
	{
		printf("CSDB base path = %s\nChecking source files...\n",
			m_csdbp.getBasePath());
	}

	res = m_csdbp.setup_srcfil_read();
	if (res != csdbparser::resOK) return res;
	
	long int num = 0;
	std::string s;
	do
		{
			m_csdbp.get_next_srcfil(&s);
			if (s.length() == 0) break;
			num++;
		}
	while (s.length() > 0);
	if (m_debug)
	{
		printf("Number of source files = %ld\nReading symbols...\n", num);
	}
	
	res = m_csdbp.setup_symbol_read();
	if (res != csdbparser::resOK) return res;

	num = 0;
	symdata_pack sp;
	sp.valid = true;
	while(sp.valid)
	{
		res = m_csdbp.get_next_symbol(&sp);
		if (res != csdbparser::resOK)
		{
			if (m_debug) {printf("Error at symbol %ld, retval=%d,"
			" file=%s:%ld\n", num, res, sp.filename.c_str(), sp.line_num);}
			return res;
		}
		if (sp.line_num == -1) continue; // empty line
		if (sp.valid) num += sp.symbols.size();
	}

	if (m_debug)
	{
		printf("Total number of symbols found = %ld\nCSDB file OK!\n",num);
	}

	m_csdbp.setDebug(false);
	return csdbparser::resOK;
}

cs2sq::enResult cs2sq::open_db(const char* sqldb)
{
	int rc;
	rc = sqlite3_open_v2(sqldb, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_close(m_db);
		m_db = 0;
		return resFILE_ACCESS_ERR;
	}
	return resOK;
}

void cs2sq::close_db(void)
{
	sqlite3_reset(m_filesstmt);
	sqlite3_reset(m_linesstmt);
	sqlite3_reset(m_callstmt);
	sqlite3_reset(m_symstmt);
	sqlite3_finalize(m_filesstmt);
	sqlite3_finalize(m_linesstmt);
	sqlite3_finalize(m_callstmt);
	sqlite3_finalize(m_symstmt);
	sqlite3_close(m_db);
	m_db = 0;
}

cs2sq::enResult cs2sq::setup_tables(void)
{
	std::string s;
	//enResult res;
	int rc;
	if (m_db == NULL) return resOTHER_ERR;
	if (m_csdbp.isFileOpen() == false) return resOTHER_ERR;
	s = "PRAGMA synchronous = OFF;";
	s+= "PRAGMA journal_mode = OFF;";
	s+= "PRAGMA locking_mode = EXCLUSIVE;";
	s+= "PRAGMA automatic_index = FALSE;";
	s+= "PRAGMA cache_size = 20000;";
	s+= "BEGIN;";
	s+= "DROP TABLE IF EXISTS symtbl;";
	s+= "DROP TABLE IF EXISTS filestbl;";
	s+= "DROP TABLE IF EXISTS linestbl;";
	s+= "DROP TABLE IF EXISTS calltbl;";
	s+= "DROP TABLE IF EXISTS inherittbl;";
	s+= "DROP TABLE IF EXISTS configtbl;";
	s+= "DROP TABLE IF EXISTS membertbl;";
	s+= "DROP INDEX IF EXISTS symNameIdx;";
	s+= "DROP INDEX IF EXISTS symName2Idx;";
	s+= "DROP INDEX IF EXISTS filePathIdx;";
	s+= "DROP INDEX IF EXISTS filePath2Idx;";
	s+= "DROP INDEX IF EXISTS callerIDIdx;";
	s+= "DROP INDEX IF EXISTS calledIDIdx;";
	s+= "DROP INDEX IF EXISTS memberIDIdx;";
	s+= "DROP INDEX IF EXISTS groupIDIdx;";
	s+= "DROP INDEX IF EXISTS parentIDIdx;";
	s+= "DROP INDEX IF EXISTS childIDIdx;";
	s+= "DROP INDEX IF EXISTS lines_fileIDIDx;";
	s+= "DROP INDEX IF EXISTS lines_linenumIDx;";
	s+= "CREATE TABLE configtbl(configKey TEXT, configVal TEXT);";
	s+= "CREATE TABLE filestbl(fileID INTEGER PRIMARY KEY ASC, filePath TEXT);";
	s+= "CREATE TABLE linestbl(lineID INTEGER PRIMARY KEY ASC, linenum INTEGER, fileID INTEGER, linetext TEXT);";
	s+= "CREATE TABLE calltbl(callerID INTEGER, calledID INTEGER);";
	s+= "CREATE TABLE inherittbl(parentID INTEGER, childID INTEGER);";
	s+= "CREATE TABLE symtbl(symID INTEGER PRIMARY KEY ASC, symName TEXT, symType TEXT, lineID INTEGER);";
	s+= "CREATE TABLE membertbl(groupID INTEGER, memberID INTEGER, memberType TEXT);";
	s+= "INSERT INTO configtbl VALUES (\"DB_MAJOR_VER\",\"0\");";
	s+= "INSERT INTO configtbl VALUES (\"DB_MINOR_VER\",\"1\");";
	s+= "INSERT INTO configtbl VALUES (\"DB_BASE_PATH\",\"";
	s+= m_csdbp.getBasePath();
	s+= "\");COMMIT;";
	rc=sqlite3_exec(m_db, s.c_str(), NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr1: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	return resOK;
}

cs2sq::enResult cs2sq::add_symbols(void)
{
	enResult res;
	int rc;
	if (m_db == NULL) return resOTHER_ERR;
	if (m_csdbp.isFileOpen() == false) return resOTHER_ERR;
	m_csdbpLastErr = m_csdbp.setup_symbol_read();
	if (m_csdbpLastErr != csdbparser::resOK) return resCSDBPError;
	rc = prepare_stmt(&m_filesstmt, "INSERT INTO filestbl VALUES (?,?);");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_linesstmt, "INSERT INTO linestbl VALUES (?,?,?,?);");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_callstmt, "INSERT INTO calltbl VALUES (?,?);");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_symstmt, "INSERT INTO symtbl VALUES (?,?,?,?);");
	if (rc!=0) return resSQLError;
	rc=sqlite3_exec(m_db, "BEGIN EXCLUSIVE;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr13: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	
	idxcounter fileidx;
	idxcounter symidx;
	idxcounter lineidx;
	std::string current_file = "";
	std::string s;
	symdata_pack sp;
	sp.valid = true;
	while(sp.valid)
	{
		m_csdbpLastErr = m_csdbp.get_next_symbol(&sp);
		if (m_csdbpLastErr != csdbparser::resOK) return resCSDBPError;
		if (sp.valid == false) break; //end of symbols
		if (sp.line_num == -1) continue; // empty line
		++lineidx;
		if (current_file.compare(sp.filename) != 0)
		{
			++fileidx;
			current_file = sp.filename;
			rc=execstmt(m_filesstmt, fileidx.getStr(), current_file.c_str());
			if (rc!=0) return resSQLError;
		}
		rc=execstmt(m_linesstmt, lineidx.getStr(), sp.line_num_str().c_str(),
						fileidx.getStr(), sp.line_text_replacetab().c_str());
		if (rc!=0) return resSQLError;
		if (sp.symbols.empty() == false)
			res = add_symdata(sp.symbols, lineidx.getStr(), &symidx);
		if (res != resOK) return res;
		
	}
	return resOK;
}

cs2sq::enResult cs2sq::add_symdata(symdatalist sdlist, const char* lineid, idxcounter* idx)
{
	//enResult res;
	symdatalist::iterator it;
	char smallstr[2];
	int rc;
	smallstr[1] = 0; // NULL-terminated
	for (it = sdlist.begin(); it < sdlist.end(); it++)
	{
		++(*idx);
		smallstr[0] = it->getTypeChar();
		rc=execstmt(m_symstmt, idx->getStr(), it->symbname.c_str(),
							smallstr, lineid);
		if (rc!=0) return resSQLError;
		if ((it->sym_type != sym_data::symMacroDef)&&
			(it->sym_type != sym_data::symFuncCall)&&
			(it->sym_type != sym_data::symFuncDef))
			continue;
		switch (it->sym_type)
		{
			case sym_data::symMacroDef:
				m_calling_macro.str = it->symbname;
				m_calling_macro.id = idx->getStr();
				break;
			case sym_data::symFuncDef:
				m_calling_func.str = it->symbname;
				m_calling_func.id = idx->getStr();
				break;
			case sym_data::symFuncCall:
				if ((it->calling_func.empty() == false)
					&&(m_calling_func.str.compare(it->calling_func) == 0))
				{
					rc=execstmt(m_callstmt, m_calling_func.id.c_str(), idx->getStr());
					if (rc!=0) return resSQLError;
				}
				if ((it->calling_macro.empty() == false)
					&&(m_calling_macro.str.compare(it->calling_macro) == 0))
				{
					rc=execstmt(m_callstmt, m_calling_macro.id.c_str(), idx->getStr());
					if (rc!=0) return resSQLError;
				}
				break;
			default: break;
		};
	}
	return resOK;
}

cs2sq::enResult cs2sq::finalize(void)
{
	int rc=sqlite3_exec(m_db, "COMMIT;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr14: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	std::string s;
	s  = "BEGIN EXCLUSIVE;";
	s += "CREATE INDEX filePathIdx ON filestbl (filePath);";
	s += "CREATE INDEX filePath2Idx ON filestbl (filePath COLLATE NOCASE);";
	s += "CREATE INDEX callerIDIdx ON calltbl (callerID);";
	s += "CREATE INDEX calledIDIdx ON calltbl (calledID);";
	s += "CREATE INDEX lines_fileIDIDx ON linestbl (fileID);";
	s += "CREATE INDEX lines_linenumIDx ON linestbl (linenum);";
	s += "CREATE INDEX symNameIdx ON symtbl (symName, symType);";
	s += "CREATE INDEX symName2Idx ON symtbl (symName COLLATE NOCASE, symType COLLATE NOCASE);";
	//s += "VACUUM;";
	s += "COMMIT;";
	rc=sqlite3_exec(m_db, s.c_str(), NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr15: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	return resOK;
}


