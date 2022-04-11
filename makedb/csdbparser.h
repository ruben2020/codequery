
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
#define CSDBP_SUPPORTED_VER_NUM  (15)
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

void create_buf(long int size = CSDBP_MINIM_BUFSIZE);
void destroy_buf(void);
enResult parse_headers(void);
enResult single_line_symbol(bool& endOfSymbData, bool& foundSomething);
enResult symbolread(sym_data* data, symdata_pack* pack);
bool srcfil_trailer_check(void);

}; // class csdbparser


#endif //CSDBPARSER_H_CQ


