
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef CS2SQ_H_CQ
#define CS2SQ_H_CQ

#include <stdio.h>
#include <sqlite3.h>
#include "sqlbase.h"
#include "csdbparser.h"
#include "small_lib.h"

typedef struct
{
	std::string str;
	std::string id;
} stStrID;

typedef std::vector<stStrID> strIDList;

class cs2sq : public sqlbase
{
public:

enum enResult
{
	resOK = 0,
	resSQLError,
	resCSDBPFileNotOpen,
	resCSDBPError,
	resFilenameError,
	resFILE_ACCESS_ERR,
	resOTHER_ERR
};

cs2sq();
~cs2sq();

csdbparser::enResult open_csdb(const char* csdbfn);
void close_csdb(void);
csdbparser::enResult test_csdb(void);
enResult open_db(const char* sqldb);
void close_db(void);

enResult setup_tables(void);
enResult add_symbols(void);
enResult finalize(void);

csdbparser::enResult get_csdbpError(void) {return m_csdbpLastErr;}
bool isCSDBFileOpen(void)  {return m_csdbp.isFileOpen();}

private:
char* m_buf;
csdbparser m_csdbp;
csdbparser::enResult m_csdbpLastErr;
stStrID m_calling_func;
stStrID m_calling_macro;
sqlite3_stmt* m_filesstmt;
sqlite3_stmt* m_linesstmt;
sqlite3_stmt* m_callstmt;
sqlite3_stmt* m_symstmt;
enResult add_symdata(symdatalist sdlist, const char* lineid, idxcounter* idx);
}; // class cs2sq

#endif

