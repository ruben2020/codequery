
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

#ifndef _CS2SQ_H
#define _CS2SQ_H

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
