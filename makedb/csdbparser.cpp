
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

#include <stdio.h>
#include <unistd.h>
#include "csdbparser.h"
#include "small_lib.h"
//#include "packtext.h"

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
	for (int i=0; i<symbtypetbl_SIZE; i++)
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
	for (int i=0; i<symbtypetbl_SIZE; i++)
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
	return add_escape_char(line_text.substr(0, 80) , '"', '"');
}

std::string symdata_pack::line_text_replacetab(void)
{
	std::string s;
	int i;
	for (i=0; i < line_text.length(); i++)
	{
		if (line_text[i] == '\t') s += ' ';
		else s += line_text[i];
	}
	s = s.substr(0, 80);
	return s;
}

std::string symdata_pack::line_text_blob(void)
{
	tempbuf buf(20000);
	strcpy(buf.get(), line_text.substr(0, 80).c_str());
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
,m_debug(true)
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

void csdbparser::create_buf(int size)
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
if (m_buf != NULL)
	{
	delete[] m_buf;
	m_buf = NULL;
	m_bufsize = 0;
	}
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
int hdr_len=0;
std::string s;

// Does the file exist?
if (access(fn, F_OK) == -1) {return resFILE_NOT_FOUND;}

// Try to open the file for reading
fp = fopen(fn, "r");
if (fp == NULL) {return resFILE_ACCESS_ERR;}

// Read out the first line i.e. the header
if (fgets(buf.get(), 20000, fp.get()) == NULL) {return resFILE_ACCESS_ERR;}
chomp(buf.get());

s = static_cast<const char*>(buf.get());
hdr_len = strlen(buf.get());

// Header should be at least 19 bytes long and starts with "cscope "
if ((hdr_len<19)||(s.compare(0, strlen("cscope "), "cscope ") != 0))
     {return resUNRECOG_FORMAT;}

// Compare the cscope version used to build the database
if (s.compare(7, strlen(CSDBP_SUPPORTED_VER), CSDBP_SUPPORTED_VER) != 0)
     {return resINCORRECT_VER;}

// Compare the parameters used to build the database with the supported one
if (s.compare(hdr_len - 28, strlen(CSDBP_SUP_PARAM), CSDBP_SUP_PARAM) != 0)
if (s.compare(hdr_len - 14, strlen(CSDBP_SUP_PARAM2), CSDBP_SUP_PARAM2) != 0)
     {return resUNSUPPORTED_PARAM;}

// Trailer offset should be a positive number, normally > 20
if (atol(s.substr(hdr_len - 10).c_str()) < 20) {return resUNRECOG_FORMAT;}

// Header looks OK so far!

return resOK;
}

csdbparser::enResult csdbparser::parse_headers(void)
{
int slen;

if (m_fp == NULL) {return resFILE_NOT_OPEN;}
create_buf();

m_base_path.clear();
m_calling_func.clear();
m_calling_macro.clear();
m_current_srcfile.clear();

// Read out the first line i.e. the header
if (fgets(m_buf, CSDBP_MINIM_BUFSIZE, m_fp) == NULL) {return resFILE_ACCESS_ERR;}
slen = strlen(chomp(m_buf));

std::string s(static_cast<const char*>(m_buf));
m_trailer_start = atol(s.substr(slen - 10).c_str());

// Remove the part of the string after the base path
if (s.compare(slen - 28, strlen(CSDBP_SUP_PARAM), CSDBP_SUP_PARAM) == 0)
	m_buf[slen - 28] = 0; 
else m_buf[slen - 14] = 0;

//slen = strlen(m_buf);

s = static_cast<const char*>(m_buf);
m_base_path = s.substr(strlen("cscope ") + strlen(CSDBP_SUPPORTED_VER) + 1);
if (*(m_base_path.rbegin()) == '\"') m_base_path.erase(m_base_path.end() - 1);

return resOK;
}

csdbparser::enResult csdbparser::setup_srcfil_read(void)
{
CSDBP_GENERAL_CHK();

long int num;

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

int loopcheck = 0; // prevent infinite loop
sym_data sd;
while(loopcheck++ < 65500)
	{
		// symbol line
		ch = fgetc(m_fp);
		if ((ch == 0x0A)&&(loopcheck > 1))
		{
			break; //EOL
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
				for (int i=0; i<symbtypetbl_SIZE; i++)
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


