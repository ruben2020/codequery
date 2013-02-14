
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

#include "ctagread.h"
#include "small_lib.h"

ctagread::ctagread()
:f_tags(NULL)
,m_insertstmt(NULL)
,m_readclassstmt(NULL)
,m_readsymstmt(NULL)
,m_writedeststmt(NULL)
,m_readsymfstmt(NULL)
{
}

ctagread::~ctagread()
{
	close_files();
}

ctagread::enResult ctagread::open_files(const char* sqldb, const char* tagsfn)
{
	int rc;
	f_tags = fopen(tagsfn, "r");
	if (f_tags == NULL) return resFILE_ACCESS_ERR;
	
	rc = sqlite3_open_v2(sqldb, &m_db, SQLITE_OPEN_READWRITE, NULL);
	if (rc != SQLITE_OK)
	{
		sqlite3_close(m_db);
		return resFILE_ACCESS_ERR;
	}
	rc=sqlite3_exec(m_db, "PRAGMA synchronous = OFF;\
		PRAGMA journal_mode = OFF;\
		PRAGMA locking_mode = EXCLUSIVE;\
		PRAGMA automatic_index = FALSE;", NULL, 0, NULL);
	if (rc != SQLITE_OK) return resSQLError;
	return resOK;
}

void ctagread::close_files(void)
{
	sqlite3_reset(m_insertstmt);
	sqlite3_reset(m_readclassstmt);
	sqlite3_reset(m_readsymstmt);
	sqlite3_reset(m_writedeststmt);
	sqlite3_reset(m_readsymfstmt);
	sqlite3_finalize(m_insertstmt);
	sqlite3_finalize(m_readclassstmt);
	sqlite3_finalize(m_readsymstmt);
	sqlite3_finalize(m_writedeststmt);
	sqlite3_finalize(m_readsymfstmt);
	m_insertstmt = NULL;
	m_readclassstmt = NULL;
	m_readsymstmt = NULL;
	m_writedeststmt = NULL;
	m_readsymfstmt = NULL;
	fclose(f_tags);
	f_tags = NULL;
	sqlite3_close(m_db);
}

typedef struct
{
	std::string cls;
	std::string id;
} stClsID;


ctagread::enResult ctagread::process_ctags(void)
{
	tempbuf sym(200), fil(200), classname(200), numtxt(30), linetxt(2000), fil2(200);
	long int num;
	int numOfLines=0;
	char* retval;
	int scanretval = 0;
	int rc;
	char c;
	char smallstr[2];
	char *cp;
	strctagIDList classIDs, symIDs;
	enResult res;
	std::vector<stClsID> listClsHist;
	
	*(fil.get()) = '%'; // for SQL LIKE pattern recognition
	smallstr[1] = 0;
	rc = prepare_stmt(&m_insertstmt, "INSERT INTO membertbl VALUES (?,?,?);");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_readclassstmt, "SELECT symID FROM symtbl WHERE symName=? AND symType=\"c\";");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_readsymstmt, "SELECT symID FROM symtbl WHERE symName=? AND lineid IN (SELECT lineID FROM linestbl WHERE linenum=? AND fileid IN (SELECT fileID FROM filestbl WHERE filePath LIKE ?));");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_writedeststmt, "UPDATE symtbl SET symName=? WHERE symID=?;");
	if (rc!=0) return resSQLError;
	rc = prepare_stmt(&m_readsymfstmt, "SELECT symID FROM symtbl WHERE symName=? AND symType=\"$\" AND lineid IN (SELECT lineID FROM linestbl WHERE linenum=? AND fileid IN (SELECT fileID FROM filestbl WHERE filePath LIKE ?));");
	if (rc!=0) return resSQLError;
	rc=sqlite3_exec(m_db,  "DROP INDEX IF EXISTS memberIDIdx;\
							DROP INDEX IF EXISTS groupIDIdx;\
							DELETE FROM membertbl;\
							BEGIN EXCLUSIVE;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr13: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	
	do{
		retval = fgets(linetxt.get(), 2000, f_tags);
		if (retval != NULL)
		{
			chomp(linetxt.get());
			scanretval = sscanf(linetxt.get(), "%s\t%s\t%ld;\"\t%c\tclass:%s", sym.get(), fil2.get(), &num, &c, classname.get());
		}
		if ((retval != NULL)&&(scanretval == 5))
		{
			classIDs.clear();
			strcpy(fil.get(), "%");
			strcat(fil.get(), extract_filename(fil2.get()));
			for (int i=0; i<listClsHist.size(); i++)
			{
				if (listClsHist[i].cls.compare(classname.get()) == 0)
				{classIDs.push_back(listClsHist[i].id); break;}
			}
			if (classIDs.empty())
			{
				res = getListOfClassIDs(&classIDs, classname.get());
				if (res != resOK) {return res;}
				if (classIDs.empty() == false)
				{
					stClsID tempClsID;
					tempClsID.cls = classname.get();
					tempClsID.id = classIDs[0];
					listClsHist.insert(listClsHist.begin(), tempClsID);
					if (listClsHist.size() > 50) listClsHist.pop_back();
				}
			}
			if (classIDs.empty()) continue;
			cp = sym.get();
			if (*(sym.get()) == '~')
			{
				cp = (sym.get()) + 1; //include destructors
									// which cscope missed out				
			}
			sprintf(numtxt.get(), "%ld", num);
			if (c == 'f')
				res = getListOfSymIDs(m_readsymfstmt, &symIDs, cp, numtxt.get(), fil.get());
			else
				res = getListOfSymIDs(m_readsymstmt, &symIDs, cp, numtxt.get(), fil.get());
			if (res != resOK) {return res;}
			if (symIDs.empty() == false)
			{
				for (int i=0; i < symIDs.size(); i++)
					{
						smallstr[0] = c;
						rc=execstmt(m_insertstmt, classIDs[0].c_str(), symIDs[i].c_str(), smallstr);
						if (rc!=0) return resSQLError;						
						if (*(sym.get()) == '~')
						{
							rc=execstmt(m_writedeststmt, sym.get(), symIDs[i].c_str());
							if (rc!=0) return resSQLError;
						}
						numOfLines++;
					}
			}
			//else {if (m_debug) {printf("no match found for symbol: %s\n",sym.get());}}
		}
	} while (retval != NULL);
	if (m_debug) printf ("Total membertbl records possible = %d\n", numOfLines);
	return resOK;
}

ctagread::enResult ctagread::getListOfClassIDs(strctagIDList* idlist, const char* v1)
{

int rc;
std::string s;

idlist->clear();
rc = execstmt(m_readclassstmt, v1);
while ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY))
	{
		if (rc == SQLITE_ROW)
		{
			s = (const char*) sqlite3_column_text(m_readclassstmt, 0);
			idlist->push_back(s);
		}
		rc = sqlite3_step(m_readclassstmt);
	}
return resOK;
}

ctagread::enResult ctagread::getListOfSymIDs(sqlite3_stmt* pstmt, strctagIDList* idlist, const char* v1, const char* v2, const char* v3)
{

int rc;
std::string s;

idlist->clear();
rc = execstmt(pstmt, v1, v2, v3);
while ((rc == SQLITE_ROW)||(rc == SQLITE_BUSY))
	{
		if (rc == SQLITE_ROW)
		{
			s = (const char*) sqlite3_column_text(pstmt, 0);
			idlist->push_back(s);
		}
		rc = sqlite3_step(pstmt);
	}
return resOK;
}

ctagread::enResult ctagread::finalize(void)
{
	int rc=sqlite3_exec(m_db, "COMMIT;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr14: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	std::string s;
	s  = "BEGIN EXCLUSIVE;";
	s += "CREATE INDEX groupIDIdx ON membertbl (groupID);";
	s += "CREATE INDEX memberIDIdx ON membertbl (memberID);";
	s += "REINDEX symNameIdx;";
	s += "COMMIT;";
	rc=sqlite3_exec(m_db, s.c_str(), NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr15: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	return resOK;
}

