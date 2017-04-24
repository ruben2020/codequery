
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef CTAGREAD_H_CQ
#define CTAGREAD_H_CQ

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
sqlite3_stmt* m_writetypestmt;
enResult getListOfClassIDs(strctagIDList* idlist, const char* v1);
enResult getHListOfClassIDs(strctagIDList* idlist, const char* v1, std::vector<stClsID> *listClsHist);
enResult getListOfSymIDs(sqlite3_stmt* pstmt, strctagIDList* idlist, const char* v1, const char* v2, const char* v3);
enResult prepare_cqdb(void);

};

#endif

