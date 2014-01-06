
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


#ifndef _CTAGREAD_H
#define _CTAGREAD_H

#include <string>
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "sqlbase.h"

typedef std::vector<std::string> strctagIDList;

typedef struct
{
	std::string cls;
	std::string id;
} stClsID;

class ctagread : public sqlbase
{

public:

enum enResult
  {
   resOK = 0,
   resSQLError,
   resFILE_NOT_FOUND,
   resFILE_ACCESS_ERR,
   resUNRECOG_FORMAT,
   resINCORRECT_VER,
   resUNSUPPORTED_PARAM,
   resOTHER_ERR,
   resUNKNOWN_ERR,
   resFILE_NOT_OPEN
  };

ctagread();
~ctagread();
enResult open_files(const char* sqldb, const char* tagsfn);
void close_files(void);
enResult process_ctags(void);
enResult finalize(void);

private:
FILE* f_tags;
sqlite3_stmt* m_insertstmt;
sqlite3_stmt* m_insertinheritstmt;
sqlite3_stmt* m_readclassstmt;
sqlite3_stmt* m_readsymstmt;
sqlite3_stmt* m_readsymfstmt;
sqlite3_stmt* m_writedeststmt;
enResult getListOfClassIDs(strctagIDList* idlist, const char* v1);
enResult getHListOfClassIDs(strctagIDList* idlist, const char* v1, std::vector<stClsID> *listClsHist);
enResult getListOfSymIDs(sqlite3_stmt* pstmt, strctagIDList* idlist, const char* v1, const char* v2, const char* v3);
enResult prepare_cqdb(void);

};

#endif
