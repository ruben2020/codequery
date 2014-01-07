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
,m_insertinheritstmt(NULL)
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
	rc=sqlite3_exec(m_db, "PRAGMA synchronous = OFF;"
		"PRAGMA journal_mode = OFF;"
		"PRAGMA locking_mode = EXCLUSIVE;"
		"PRAGMA automatic_index = FALSE;"
		"PRAGMA cache_size = 20000;", NULL, 0, NULL);
	if (rc != SQLITE_OK) return resSQLError;
	return resOK;
}

void ctagread::close_files(void)
{
	sqlite3_reset(m_insertstmt);
	sqlite3_reset(m_insertinheritstmt);
	sqlite3_reset(m_readclassstmt);
	sqlite3_reset(m_readsymstmt);
	sqlite3_reset(m_writedeststmt);
	sqlite3_reset(m_readsymfstmt);
	sqlite3_finalize(m_insertstmt);
	sqlite3_finalize(m_insertinheritstmt);
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

ctagread::enResult ctagread::prepare_cqdb(void)
{
	int rc;

	rc = prepare_stmt(&m_insertstmt, "INSERT INTO membertbl VALUES (?,?,?);");
	if (rc != SQLITE_OK) return resSQLError;

	rc = prepare_stmt(&m_insertinheritstmt, "INSERT INTO inherittbl VALUES (?,?);");
	if (rc != SQLITE_OK) return resSQLError;

	rc = prepare_stmt(&m_readclassstmt, "SELECT symID FROM symtbl WHERE symName=? AND symType=\"c\";");
	if (rc != SQLITE_OK) return resSQLError;

	//rc = prepare_stmt(&m_readsymstmt, "SELECT symID FROM symtbl WHERE symName=? AND lineid IN (SELECT lineID FROM linestbl WHERE linenum=? AND fileid IN (SELECT fileID FROM filestbl WHERE filePath LIKE ?));");
	rc = prepare_stmt(&m_readsymstmt, "SELECT symtbl.symID FROM symtbl INNER JOIN linestbl ON (symtbl.symName=? AND symtbl.lineID = linestbl.lineID AND linestbl.linenum=?) INNER JOIN filestbl ON (linestbl.fileID = filestbl.fileID AND filePath LIKE ?);");
	if (rc != SQLITE_OK) return resSQLError;

	rc = prepare_stmt(&m_writedeststmt, "UPDATE symtbl SET symName=? WHERE symID=?;");
	if (rc != SQLITE_OK) return resSQLError;

	//rc = prepare_stmt(&m_readsymfstmt, "SELECT symID FROM symtbl WHERE symName=? AND symType=\"$\" AND lineid IN (SELECT lineID FROM linestbl WHERE linenum=? AND fileid IN (SELECT fileID FROM filestbl WHERE filePath LIKE ?));");
	rc = prepare_stmt(&m_readsymfstmt, "SELECT symtbl.symID FROM symtbl INNER JOIN linestbl ON (symtbl.symName=? AND symtbl.symType=\"$\" AND symtbl.lineID = linestbl.lineID AND linestbl.linenum=?) INNER JOIN filestbl ON (linestbl.fileID = filestbl.fileID AND filePath LIKE ?);");
	if (rc != SQLITE_OK) return resSQLError;

	rc=sqlite3_exec(m_db,   "BEGIN EXCLUSIVE;"
				"DROP INDEX IF EXISTS memberIDIdx;"
				"DROP INDEX IF EXISTS groupIDIdx;"
				"DROP INDEX IF EXISTS parentIDIdx;"
				"DROP INDEX IF EXISTS childIDIdx;"
				"DELETE FROM membertbl;"
				"DELETE FROM inherittbl;"
				"COMMIT;", NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr13: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}

	return resOK;
}

ctagread::enResult ctagread::process_ctags(void)
{
	tempbuf sym(400), fil(500), classname(400), classname2(400);
	tempbuf numtxt(50), linetxt(4001), fil2(500);
	long int num;
	int numOfLines=0;
	int numOfLines2=0;
	char* retval;
	int scanretval = 0;
	int rc;
	char c;
	char smallstr[2];
	char *cp;
	strctagIDList classIDs, symIDs, parentClassIDs, parentClassIDs_temp;
	enResult res;
	std::vector<stClsID> listClsHist;
	bool result;

	*(fil.get()) = '%'; // for SQL LIKE pattern recognition
	smallstr[1] = 0;
	res = prepare_cqdb();
	if (res != resOK) return res;
	
	do{
		retval = fgets(linetxt.get(), linetxt.size() - 1, f_tags);
		if (retval != NULL)
		{
			chomp(linetxt.get());
			scanretval = sscanf(linetxt.get(), "%s\t%s\t%ld;\"\t%c\tclass:%s", sym.get(), fil2.get(), &num, &c, classname.get());
		}
		if ((retval != NULL)&&(scanretval == 5))
		{
			strcpy(fil.get(), "%");
			strcat(fil.get(), extract_filename(fil2.get()));
			res = getHListOfClassIDs(&classIDs, get_last_part(classname.get(), '.'), &listClsHist);
			if (res != resOK) return res;
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
				for (unsigned int i=0; i < symIDs.size(); i++)
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
		else if (retval != NULL)
		{
			scanretval = sscanf(linetxt.get(),
			"%s\t%s\t%ld;\"\t%c\tinherits:%s", sym.get(), fil2.get(), &num, &c, classname.get());
			result = ((scanretval == 5)&&(c == 'c'));
			if (!result)
			{
				scanretval = sscanf(linetxt.get(),
				"%s\t%s\t%ld;\"\t%c\tfile:\tinherits:%s", sym.get(), fil2.get(),
					&num, &c, classname.get());
				result = ((scanretval == 5)&&(c == 'c'));
			}
			if (!result)
			{
				scanretval = sscanf(linetxt.get(),
				"%s\t%s\t%ld;\"\t%c\tclass:%s\tinherits:%s", sym.get(), fil2.get(),
					&num, &c, classname2.get(), classname.get());
				result = ((scanretval == 6)&&(c == 'c'));
			}
			if (result)
			{
				res = getHListOfClassIDs(&classIDs, sym.get(), &listClsHist);
				if (res != resOK) return res;
				if (classIDs.empty()) continue;
				parentClassIDs.clear();
				parentClassIDs_temp.clear();
				std::vector<std::string> vecstr = splitstr(classname.get(), ',');
				for (unsigned int i=0; i<vecstr.size(); i++)
				{
					res = getHListOfClassIDs(&parentClassIDs_temp, 
						get_last_part((char*)vecstr[i].c_str(), '.'), &listClsHist);
					if (res != resOK) return res;
					while (parentClassIDs_temp.empty() == false)
					{
						parentClassIDs.push_back(parentClassIDs_temp.back());
						parentClassIDs_temp.pop_back();
					}
				}
				for (unsigned int i=0; i<parentClassIDs.size(); i++)
				{
					rc=execstmt(m_insertinheritstmt, parentClassIDs[i].c_str(), classIDs[0].c_str());
					if (rc!=0) return resSQLError;
					numOfLines2++;
				}
			}
		}
	} while (retval != NULL);
	if (m_debug) printf ("Total membertbl records possible = %d\n", numOfLines);
	if (m_debug) printf ("Total inherittbl records possible = %d\n", numOfLines2);
	return resOK;
}

ctagread::enResult ctagread::getHListOfClassIDs(strctagIDList* idlist, const char* v1, std::vector<stClsID> *listClsHist)
{
	enResult res = resOK;
	idlist->clear();
	for (unsigned int i=0; i<listClsHist->size(); i++)
	{
		if ((*listClsHist)[i].cls.compare(v1) == 0)
		{idlist->push_back((*listClsHist)[i].id); break;}
	}
	if (idlist->empty())
	{
		res = getListOfClassIDs(idlist, v1);
		if (res != resOK) {return res;}
		if (idlist->empty() == false)
		{
			stClsID tempClsID;
			tempClsID.cls = v1;
			tempClsID.id = (*idlist)[0];
			(*listClsHist).insert((*listClsHist).begin(), tempClsID);
			if ((*listClsHist).size() > 50) (*listClsHist).pop_back();
		}
	}
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
	int rc;
	std::string s;
	s  = "BEGIN EXCLUSIVE;";
	s += "CREATE INDEX groupIDIdx ON membertbl (groupID);";
	s += "CREATE INDEX memberIDIdx ON membertbl (memberID);";
	s += "CREATE INDEX parentIDIdx ON inherittbl (parentID);";
	s += "CREATE INDEX childIDIdx ON inherittbl (childID);";
	s += "REINDEX symNameIdx;";
	s += "REINDEX symName2Idx;";
	s += "COMMIT;";
	rc=sqlite3_exec(m_db, s.c_str(), NULL, 0, NULL);
	if (rc != SQLITE_OK)
	{
		if (m_debug) printf("SQLErr15: %d, %s\n", rc, sqlite3_errmsg(m_db));
		return resSQLError;
	}
	return resOK;
}

