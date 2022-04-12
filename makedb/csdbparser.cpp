
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

#include <stdio.h>
#include "csdbparser.h"
#include "csdbheader.h"
#include "small_lib.h"

#define MAX_TEXT_LINE_LEN     (800)

#define CSDBP_GENERAL_CHK()   if ((m_fp == NULL) || \
                                  (m_trailer_start <= 0) || \
                                  (m_buf == NULL)) \
                                  {return resOTHER_ERR;}
                                  
typedef struct
{
int chr;
sym_data::enSymType type;
const char* desc;
} chr2enum;

static const chr2enum symbtypetbl[] =
{
	{'`', sym_data::symFuncCall, "FuncCall"},
	{'=', sym_data::symDrctAssgnIncrDecr, "DirectAssgnIncrDecr"},
	{'c', sym_data::symClass, "Class"},
	{'e', sym_data::symEnum, "Enum"},
	{'g', sym_data::symGlobal, "OtherGlobal"},
	{'l', sym_data::symLocalFuncBlockDef, "LocalFuncBlockDef"},
	{'m', sym_data::symGlobalEnumStructUnion, "GlobalEnumStructUnion"},
	{'p', sym_data::symFuncParam, "FuncParam"},
	{'s', sym_data::symStruct, "Struct"},
	{'t', sym_data::symTypeDef, "TypeDef"},
	{'u', sym_data::symUnion, "Union"},
	{'$', sym_data::symFuncDef, "FuncDef"},
	{'#', sym_data::symMacroDef, "MacroDef"},
	{'~', sym_data::symIncl, "Include"},
	{'@', sym_data::symFile, "File"},
	{'X', sym_data::symNone, "NoType"}
};
#define symbtypetbl_SIZE (sizeof(symbtypetbl)/sizeof(chr2enum))

// sym_data ///////////////////////////////////////////////////

sym_data::sym_data()
:valid(false)
,sym_type(symNone)
{
}

sym_data::sym_data(const sym_data& copy)
{
valid = copy.valid;
symbname = copy.symbname;
sym_type = copy.sym_type;
calling_func = copy.calling_func;
calling_macro = copy.calling_macro;
}

sym_data& sym_data::operator= (const sym_data& copy)
{
if (this != &copy)
	{	
		valid = copy.valid;
		symbname = copy.symbname;
		sym_type = copy.sym_type;
		calling_func = copy.calling_func;
		calling_macro = copy.calling_macro;
	}
return *this;
}

void sym_data::clear(void)
{
valid = false;
symbname.clear();
sym_type = symNone;
calling_func.clear();
calling_macro.clear();
}

std::string sym_data::symbname_escaped(void)
{
	return add_escape_char(symbname, '"', '"');
}

const char* sym_data::getTypeDesc(void)
{
	for (unsigned long i=0; i<symbtypetbl_SIZE; i++)
	{
		if (sym_type == symbtypetbl[i].type)
		{
			return symbtypetbl[i].desc;
		}
	}
	return "";
}

const char sym_data::getTypeChar(void)
{
	for (unsigned long i=0; i<symbtypetbl_SIZE; i++)
	{
		if (sym_type == symbtypetbl[i].type)
		{
			return static_cast<const char>(symbtypetbl[i].chr);
		}
	}
	return '\0';
}
// symdata_pack ///////////////////////////////////////////////////

symdata_pack::symdata_pack()
:valid(false)
,line_num(0)
{
}

symdata_pack::symdata_pack(const symdata_pack& copy)
{
valid = copy.valid;
filename = copy.filename;
line_num = copy.line_num;
line_text = copy.line_text;
symbols = copy.symbols;
}

symdata_pack& symdata_pack::operator= (const symdata_pack& copy)
{
if (this != &copy)
	{
		valid = copy.valid;
		filename = copy.filename;
		line_num = copy.line_num;
		line_text = copy.line_text;
		symbols = copy.symbols;
	}
return *this;
}

void symdata_pack::clear(void)
{
valid = false;
filename.clear();
line_num = 0;
line_text.clear();
symbols.clear();
}

std::string symdata_pack::line_num_str(void)
{
	char str[30];
	sprintf(str, "%ld", line_num);
	std::string s(str);
	return s;
}

std::string symdata_pack::line_text_escaped(void)
{
	return add_escape_char(line_text.substr(0, MAX_TEXT_LINE_LEN) , '"', '"');
}

std::string symdata_pack::line_text_replacetab(void)
{
	std::string s;
	long i;
	long n = line_text.length();
	for (i=0; i < n; i++)
	{
		if (line_text[i] == '\t') s += ' ';
		else s += line_text[i];
	}
	s = s.substr(0, MAX_TEXT_LINE_LEN);
	return s;
}

std::string symdata_pack::line_text_blob(void)
{
	tempbuf buf(20000);
	strcpy(buf.get(), line_text.substr(0, MAX_TEXT_LINE_LEN).c_str());
	//packtext(buf.get(), true);
	std::string s(buf.get());
	return s;
}

// csdbparser ///////////////////////////////////////////////////

csdbparser::csdbparser()
: m_fp(NULL)
,m_buf(NULL)
,m_state(stIDLE)
,m_trailer_start(0)
,m_bufsize(0)
,m_debug(false)
{
}

csdbparser::~csdbparser()
{
close_file();
}

const char* csdbparser::getBasePath(void)
{
	return m_base_path.c_str();
}

void csdbparser::setDebug(bool val)
{
	m_debug = val;
}

void csdbparser::create_buf(long int size)
{
	if (size >= CSDBP_MINIM_BUFSIZE)
	{
		destroy_buf();
		m_buf = new char[size];
		*m_buf = 0;
		m_bufsize = size;
	}
}

void csdbparser::destroy_buf(void)
{
	delete[] m_buf;
	m_buf = NULL;
	m_bufsize = 0;
}

csdbparser::enResult csdbparser::open_file(const char *fn)
{

enResult res = file_sanity_check(fn);
if (res != resOK) {return res;}

close_file();
m_fp = fopen(fn, "r");
res = parse_headers();
if (res != resOK) {close_file();}
return res;
}

void csdbparser::close_file(void)
{
if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
m_trailer_start = 0;
destroy_buf();
m_base_path.clear();
m_calling_func.clear();
m_calling_macro.clear();
m_current_srcfile.clear();
}

csdbparser::enResult csdbparser::file_sanity_check(const char *fn)
{
smartFILE fp;
tempbuf buf(20000);
long i;
std::string s;
bool chkok;

// Does the file exist?
if (check_fileExists(fn) == false) {return resFILE_NOT_FOUND;}

// Try to open the file for reading
fp = fopen(fn, "r");
if (fp == NULL) {return resFILE_ACCESS_ERR;}

// Read out the first line i.e. the header
if (fgets(buf.get(), 20000, fp.get()) == NULL) {return resFILE_ACCESS_ERR;}
chomp(buf.get());

s = static_cast<const char*>(buf.get());
csdbheader hdr;

hdr.set_header(s);
if (hdr.parse() == false)
     {return resUNRECOG_FORMAT;}

// Compare the cscope version used to build the database
if (hdr.get_version() != CSDBP_SUPPORTED_VER_NUM)
     {return resINCORRECT_VER;}

// Compare the parameters used to build the database with the supported one
// We must have "c", we don't mind "q" and we cannot have any other
chkok = false;
tVecStr vs = hdr.get_param_list();
for(i=0; i< (long) vs.size(); i++)
{
	if (vs[i].compare("c") == 0) chkok = true;
}
if (chkok == false) {return resUNSUPPORTED_PARAM;}

for(i=0; i< (long) vs.size(); i++)
{
	if ((vs[i].compare("c") != 0) && 
		(vs[i].compare("q") != 0)) chkok = false;
}
if (chkok == false) {return resUNSUPPORTED_PARAM;}

// Trailer offset should be a positive number, normally > 20
if (hdr.get_trailer_start() < 20) {return resUNRECOG_FORMAT;}

// Header looks OK so far!

return resOK;
}

csdbparser::enResult csdbparser::parse_headers(void)
{

if (m_fp == NULL) {return resFILE_NOT_OPEN;}
create_buf();

m_base_path.clear();
m_calling_func.clear();
m_calling_macro.clear();
m_current_srcfile.clear();

csdbheader hdr;

// Read out the first line i.e. the header
if (fgets(m_buf, CSDBP_MINIM_BUFSIZE, m_fp) == NULL) {return resFILE_ACCESS_ERR;}

std::string s(static_cast<const char*>(m_buf));

hdr.set_header(s);
if (hdr.parse() == false)
	{return resUNRECOG_FORMAT;}

m_trailer_start = hdr.get_trailer_start();
m_base_path = hdr.get_base_path();


return resOK;
}

bool csdbparser::srcfil_trailer_check(void)
{
char str[5];
char *s = nullptr;
int ret=0x30;
long int i = 0-3;

if (fseek(m_fp, m_trailer_start-3, SEEK_SET) != 0) return false;
s = fgets(str, 4, m_fp);
if (s == nullptr) return false;
if ((str[0] == 0x09)&&(str[1] == 0x40)&&(str[2] == 0x0A)) return true;

printf("WARNING: Trailer start offset is wrong. Attempting to find right trailer start offset.\n");
if (fseek(m_fp, m_trailer_start-3, SEEK_SET) != 0) return false;
while(ret != EOF)
	{
		ret = fgetc(m_fp); i++;
		if (ret != 0x09) continue;
		ret = fgetc(m_fp); i++;
		if (ret != 0x40) continue;
		ret = fgetc(m_fp); i++;
		if (ret == 0x0A)
		{
			printf("Wrong trailer start offset = %ld, Corrected trailer start offset = %ld\n", m_trailer_start, m_trailer_start + i);
			m_trailer_start += i;
			return true;
		}
	}
return false;
}

csdbparser::enResult csdbparser::setup_srcfil_read(void)
{
CSDBP_GENERAL_CHK();

long int num;

if (srcfil_trailer_check() == false) {return resOTHER_ERR;}
if (fseek(m_fp, m_trailer_start, SEEK_SET) != 0) {return resFILE_ACCESS_ERR;}

fscanf(m_fp, "%ld\n", &num); // number of source directories
while (num-- > 0) {fgets(m_buf, CSDBP_MINIM_BUFSIZE, m_fp);}
fscanf(m_fp, "%ld\n", &num); // number of include directories
while (num-- > 0) {fgets(m_buf, CSDBP_MINIM_BUFSIZE, m_fp);}

fscanf(m_fp, "%ld\n", &num); // number of files
fscanf(m_fp, "%ld\n", &num); // string size required
create_buf(num);

return resOK;
}

csdbparser::enResult csdbparser::get_next_srcfil(std::string* srcfil)
{
CSDBP_GENERAL_CHK();


if (ftell(m_fp) < m_trailer_start) setup_srcfil_read();

if (fgets(m_buf, m_bufsize, m_fp) != NULL)
     *srcfil = chomp(m_buf);
else srcfil->clear();

return resOK;
}

csdbparser::enResult csdbparser::setup_symbol_read(void)
{
CSDBP_GENERAL_CHK();

rewind(m_fp);
if (fgets(m_buf, m_bufsize, m_fp) == NULL) {return resFILE_ACCESS_ERR;}
m_calling_func.clear();
m_calling_macro.clear();
m_current_srcfile.clear();
//m_state = stSYMB_SETUP_DONE;

return resOK;
}

csdbparser::enResult csdbparser::get_next_symbol(symdata_pack* pack)
{
CSDBP_GENERAL_CHK();
enResult res;
bool endOfSymbData;
bool foundSomething;
int ch;

//if (m_state != stSYMB_SETUP_DONE) return resUNKNOWN_ERR;

pack->clear();
if (m_debug) printf("=====> get_next_symbol\n");
do
{
	res = single_line_symbol(endOfSymbData, foundSomething);
	if (res != resOK) return res;
	if (endOfSymbData)
		{
			pack->valid = false;
			if (m_debug) printf("End of symbols data!\n");
			return resOK;
		}
} while (foundSomething);
pack->valid = true;
pack->filename = m_current_srcfile;
if (m_debug) printf("=====> Back from get_next_symbol\n");

ch = fgetc(m_fp);
if (ch == 0x0A)
	{
		pack->line_num = -1; // empty line
		return resOK; //EOL
	}
else ungetc(ch, m_fp);

if (fscanf(m_fp, "%ld", &(pack->line_num)) != 1)
	{
		return resUNKNOWN_ERR;
	}
ch = fgetc(m_fp); // the space after the line number
if (fgets(m_buf, m_bufsize, m_fp) == NULL) {return resFILE_ACCESS_ERR;}
pack->line_text = chomp(m_buf);
if (m_debug) {printf("fn = %s, lineno=%ld, firstline=%s\n",
		pack->filename.c_str(), pack->line_num, pack->line_text.c_str());}

unsigned int loopcheck = 0; // prevent infinite loop
sym_data sd;
while(loopcheck++ < 65500)
	{
		// symbol line
		ch = fgetc(m_fp);
		if ((ch == 0x0A)&&(loopcheck > 1))
		{
			break; //EOL
		}
		else if ((ch >= 0x30)&&(ch <= 0x39)&&(loopcheck > 1))
		{
			ungetc(ch, m_fp);
			break; // symbol shouldn't start with line numbers
		}
		else 
		{
			ungetc(ch, m_fp);
			res = symbolread(&sd, pack);
			if (res != resOK) return res;
			pack->line_text += sd.symbname;
			if (sd.valid) {pack->symbols.push_back(sd);}
		}
		// no-symbol line
		ch = fgetc(m_fp);
		if ((ch == 0x0A)&&(loopcheck > 1)) {;}
		else
		{
			ungetc(ch, m_fp);
			if (fgets(m_buf, m_bufsize, m_fp) == NULL)
			{return resFILE_ACCESS_ERR;}
			pack->line_text += chomp(m_buf);
		}
	}
return resOK;
}

csdbparser::enResult csdbparser::single_line_symbol(bool& endOfSymbData, bool& foundSomething)
{
if (m_debug) printf("=====> single_line_symbol\n");
endOfSymbData = false;
foundSomething = false;
int ch;
ch = fgetc(m_fp);
if (m_debug) printf("check1!  %c\n", (char)ch);
if (ch == 9) // TAB
	{
		ch = fgetc(m_fp);
		foundSomething = true;
		if (m_debug) printf("check2!  %c\n", (char)ch);		
		switch(ch)
		{
			case '@': // start of a new file
				if (fgets(m_buf, m_bufsize, m_fp) == NULL)
					return resFILE_ACCESS_ERR;
				m_current_srcfile = chomp(m_buf);
				if (m_debug) printf("New file=%s\n", m_buf);
				endOfSymbData = m_current_srcfile.empty();
				break;
			case ')': // end of a definition
				if (m_calling_macro.size() > 0)
					m_calling_macro.clear();
				else if (m_debug) printf("no macro to clear!\n");
				if (fgets(m_buf, m_bufsize, m_fp) == NULL)
					{return resFILE_ACCESS_ERR;}
				break;
			default:
				return resUNKNOWN_ERR;
		};
		fgets(m_buf, m_bufsize, m_fp); // empty line
	}
else ungetc(ch, m_fp);
return resOK;
}

csdbparser::enResult csdbparser::symbolread(sym_data *data, symdata_pack* pack)
{
if (m_debug) printf("=====> symbolread\n");
int ch, ch2;
data->clear();
data->valid = true;
data->sym_type = sym_data::symNone;
ch = fgetc(m_fp);
if (m_debug) printf("check3!  %c\n", (char)ch);
if (ch == 9) // TAB
	{
		ch = fgetc(m_fp);
		if (m_debug) printf("check4!  %c\n", (char)ch);
		switch(ch)
		{
			case '}': // end of func
				if (m_calling_func.size() > 0)
					m_calling_func.clear();
				else if (m_debug) printf("no func to clear!\n");
				data->valid = false;
				if (m_debug) printf("End of func found\n");
				break;
			case ')': // end of macro
				if (m_calling_macro.size() > 0)
					m_calling_macro.clear();
				else if (m_debug) printf("no macro to clear!\n");
				data->valid = false;
				if (m_debug) printf("End of macro found\n");
				break;
			case '~': // include
				ch2 = fgetc(m_fp);
				if ((ch2 == '"')||(ch2 == '<'))
				{
					pack->line_text += (char) ch2;
				}
				else
				{
					ungetc(ch2, m_fp);
				}
				data->sym_type = sym_data::symIncl;
				if (m_debug) printf("Incl found\n");
				break;
			default:
				for (int i=0; i< (long) symbtypetbl_SIZE; i++)
					{
					if (symbtypetbl[i].chr == ch)
						{
						data->sym_type =
							symbtypetbl[i].type;
						break;
						}
					}
				if (data->sym_type == sym_data::symNone)
					return resUNKNOWN_ERR;
				break;
		};
	}
else ungetc(ch, m_fp);
if (fgets(m_buf, m_bufsize, m_fp) == NULL) {return resFILE_ACCESS_ERR;}
data->symbname = chomp(m_buf);
if (data->valid) data->valid = (strlen(data->symbname.c_str()) > 0);
if (m_debug) printf("sym name=%s, type = %s, valid=%d, ch=%c\n",
	data->symbname.c_str(), data->getTypeDesc(), data->valid, ch);
if      (ch == '$') {m_calling_func.assign(data->symbname);}
else if (ch == '#') {m_calling_macro.assign(data->symbname);}
else if (ch == '`')
	{
		data->calling_func = m_calling_func;
		data->calling_macro = m_calling_macro;
	}
return resOK;
}


