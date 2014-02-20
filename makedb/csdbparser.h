
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

// Cscope database parser

#ifndef CSDBPARSER_H_CQ
#define CSDBPARSER_H_CQ

#include <string>
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSDBP_MINIM_BUFSIZE      (20000)
#define CSDBP_SUPPORTED_VER      "15"
#define CSDBP_SUP_PARAM          " -c               "
#define CSDBP_SUP_PARAM2         " -c "

typedef std::vector<std::string> strvect;


// sym_data ///////////////////////////////////////////////////

class sym_data
{
public:

enum enSymType
  {
   symNone = 0,
   symFuncDef,
   symFuncCall,
   symMacroDef,
   symIncl,
   symDrctAssgnIncrDecr,
   symClass,
   symEnum,
   symGlobal,
   symLocalFuncBlockDef,
   symGlobalEnumStructUnion,
   symFuncParam,
   symStruct,
   symTypeDef,
   symUnion,
   symFile
  };

sym_data();
sym_data(const sym_data& copy);
sym_data& operator= (const sym_data& copy);
~sym_data() { /* nothing to do */ }
void clear(void);
std::string symbname_escaped(void);
const char* getTypeDesc(void);
const char getTypeChar(void);

bool valid;
std::string symbname;
enSymType sym_type;
std::string calling_func;
std::string calling_macro;
}; // class sym_data

typedef std::vector<sym_data> symdatalist;


// symdata_pack ///////////////////////////////////////////////////

class symdata_pack
{
public:
bool valid;
std::string filename;
long int line_num;
std::string line_text;
symdatalist symbols;

symdata_pack();
symdata_pack(const symdata_pack& copy);
symdata_pack& operator= (const symdata_pack& copy);
~symdata_pack() { /* nothing to do */ }
void clear(void);
std::string line_num_str(void);
std::string line_text_escaped(void);
std::string line_text_blob(void);
std::string line_text_replacetab(void);
}; // class symdata_pack


// csdbparser ///////////////////////////////////////////////////

class csdbparser
{

public:

enum enResult
  {
   resOK = 0,
   resFILE_NOT_FOUND,
   resFILE_ACCESS_ERR,
   resUNRECOG_FORMAT,
   resINCORRECT_VER,
   resUNSUPPORTED_PARAM,
   resOTHER_ERR,
   resUNKNOWN_ERR,
   resFILE_NOT_OPEN
  };

enum enState
  {
   stIDLE = 0,
   stSYMB_SETUP_DONE
  };


csdbparser();
~csdbparser();

enResult open_file(const char *fn);
bool isFileOpen(void) {return (m_fp != NULL);}
void close_file(void);
static enResult file_sanity_check(const char *fn);

enResult setup_srcfil_read(void);
enResult get_next_srcfil(std::string* srcfil);

enResult setup_symbol_read(void);
enResult get_next_symbol(symdata_pack* pack);

const char* getBasePath(void);
void setDebug(bool val);

private:
FILE *m_fp;
char *m_buf;
long int m_bufsize;
enState m_state;
long int m_trailer_start;
std::string m_base_path;
std::string m_calling_func;
std::string m_calling_macro;
std::string m_current_srcfile;
bool m_debug;

void create_buf(int size = CSDBP_MINIM_BUFSIZE);
void destroy_buf(void);
enResult parse_headers(void);
enResult single_line_symbol(bool& endOfSymbData, bool& foundSomething);
enResult symbolread(sym_data* data, symdata_pack* pack);

}; // class csdbparser


#endif //CSDBPARSER_H_CQ


