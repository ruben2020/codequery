
// Small library of useful classes and functions
//
// This license applies only to this file:
//
// Copyright (c) 2011 ruben2020 https://github.com/ruben2020/
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "small_lib.h"

bool check_fileExists(const char *fn)
{
#ifdef _MSC_VER
	DWORD dwAttrib = GetFileAttributes(fn);  
	return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && 
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
	return (access(fn, F_OK) != -1);
#endif
}

// is the file path given (fp), absolute or relative?
bool isAbsolutePath(tStr fp)
{
	bool result = false;

	// example: /usr/bin/local/file or \temp\file
	result = (fp[0] == DIRSEP);

#ifdef _WIN32
	// example: c:\temp\file
	result = result || ((fp[1] == ':')&&(fp[2] == DIRSEP));
#endif

	return result;
}

// reverse string compare
bool strrevcmp(tStr str, tStr cmpstr)
{
	bool retval = (1 == 1);
	int n = str.length();
	if (n != cmpstr.length()) {retval = (1 == 0);}
	else for (int i = (n - 1); i >= 0; i--)
	{
		if (str[i] != cmpstr[i])
		{
			retval = (1 == 0);
			break;
		}
	}
	return retval;
}

// split string into an array based on a delimiter
std::vector<std::string> splitstr(const char* inpstr, const char delim)
{
	size_t pos = 0, retpos = 0;
	std::vector<std::string> vecstr;
	std::string str(inpstr);
	do
	{
		retpos = str.find_first_of(delim, pos);
		if (retpos == std::string::npos)
		{
			vecstr.push_back(str.substr(pos));
		}
		else
		{
			vecstr.push_back(str.substr(pos, retpos - pos));
			pos = retpos + 1;
		}
	} while (retpos != std::string::npos);
	return vecstr;
}

// replace char o with char r for every part of the string
// from iterator i1 to iterator i2, excluding i2
int replacechar(std::string::iterator i1, std::string::iterator i2, const char o, const char r)
{
	int count = 0;
	for(std::string::iterator i = i1; i != i2; ++i)
	{
		if (*i == o)
		{
			*i = r;
			count++;
		}
	}
	return count;
}

// remove EOL char
const char* chomp(char* str)
{
	if ((str != NULL)&&(strlen(str) > 0))
	{
		char* charr = strrchr(str, 0xA);
		if (charr != NULL)
		{
			*charr = 0; // Unix chomp, Windows half-chomp
			charr--;
			if (*charr == 0xD) *charr = 0; // Windows chomp
		}
		else
		{
			charr = strrchr(str, 0xD);
			if (charr != NULL) *charr = 0; // MacOS chomp
		}
	}
	return static_cast<const char*>(str);
}

// add an escape character in front of another predetermined character
std::string add_escape_char(std::string ori, char chr2escp, char escpchr)
{
	std::string s;
	int num = 0;
	size_t retval = -1; 
	s = ori;
	do {
		retval = ori.find(chr2escp, retval+1);
		if (retval != std::string::npos)
		{
			s.insert(retval + num, 1, escpchr);
			num++;
		}
	} while (retval != std::string::npos);
	return s;
}

// add an escape character in front of another predetermined character
std::string add_escape_char(const char* oristr, char chr2escp, char escpchr)
{
	std::string ori(oristr);
	std::string s;
	int num = 0;
	size_t retval = -1; 
	s = ori;
	do {
		retval = ori.find(chr2escp, retval+1);
		if (retval != std::string::npos)
		{
			s.insert(retval + num, 1, escpchr);
			num++;
		}
	} while (retval != std::string::npos);
	return s;
}

// extract the filename.ext from a file path
const char* extract_filename(const char* filepath)
{
	char* str = (char*) filepath;
	char* charr = NULL;
	char pathdelim[] = {'/', '\\', ':'};

	if ((filepath != NULL)&&(strlen(filepath) > 0))
	for (int i=0; i < sizeof(pathdelim); i++)
	{
		charr = (char*) strrchr(filepath, pathdelim[i]);
		if (charr != NULL) {str = charr+1; break;}
	}
	return static_cast<const char*>(str);	
}

void smartFILE::setme(FILE *fptr) {close_file(); m_fp = fptr;}
smartFILE::smartFILE() :m_fp(NULL) {}
smartFILE::smartFILE(FILE *fptr) :m_fp(fptr) {}
smartFILE::smartFILE(const smartFILE& sfp) :m_fp(sfp.m_fp) {}
smartFILE::~smartFILE() {close_file();}
FILE* smartFILE::operator() () {return m_fp;}
bool smartFILE::operator ==(FILE* fptr) const {return (m_fp == fptr);}
bool smartFILE::operator !=(FILE* fptr) const {return (m_fp != fptr);}
bool smartFILE::operator ==(const smartFILE& sfp) const {return (m_fp == sfp.m_fp);}
bool smartFILE::operator !=(const smartFILE& sfp) const {return (m_fp != sfp.m_fp);}
smartFILE& smartFILE::operator =(FILE *fptr) {setme(fptr); return *this;}
smartFILE& smartFILE::operator =(const smartFILE& sfp)
	{
		if (this != &sfp) {setme(sfp.m_fp);}
		return *this;
	}
void smartFILE::assign(FILE* fptr) {setme(fptr);}
void smartFILE::assign(const smartFILE& sfp) {setme(sfp.m_fp);}
FILE* smartFILE::get(void) {return m_fp;}
void smartFILE::close_file(void)
	{
		if (m_fp != NULL)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
	}



tempbuf::tempbuf(unsigned int n)
	{
		if (n < 10) n = 10;
		m_buffer = new char[n];
		*m_buffer = 0;
		m_size = n;
	}
tempbuf::~tempbuf() {delete[] m_buffer;}
char* tempbuf::operator() () {return m_buffer;}
char* tempbuf::get(void) {return m_buffer;}
char  tempbuf::operator[] (unsigned int i)
	{
		char chr;
		if (i < m_size) chr = *(m_buffer + i);
		else chr = 0;
		return chr;
	}
unsigned int tempbuf::size(void) const {return m_size;}
bool tempbuf::isempty(void) const {return (*m_buffer == 0);}
void tempbuf::clear(void) {*m_buffer = 0;}
char* tempbuf::reinit(unsigned int n)
	{
		if (n < 10) n = 10;
		delete[] m_buffer;
		m_buffer = new char[n];
		*m_buffer = 0;
		m_size = n;
		return m_buffer;
	}
const char* tempbuf::constchar(void)
	{return static_cast<const char*>(m_buffer);}




idxcounter::idxcounter():m_ctr(0) {}
idxcounter::~idxcounter(){}
idxcounter::idxcounter(const idxcounter& idxc)
		{
			m_ctr = idxc.m_ctr;
		}
idxcounter& idxcounter::operator =(long unsigned int idx) {m_ctr = idx; return *this;}
idxcounter& idxcounter::operator =(const idxcounter& idxc)
		{
			if (this != &idxc)
			{
				m_ctr = idxc.m_ctr;
			}
			return *this;
		}
void idxcounter::setCounterVal(long unsigned int i) {m_ctr = i;}
void idxcounter::reset(void) {m_ctr = 0;}
idxcounter& idxcounter::operator ++() {++m_ctr;sprintf(m_buf, "%lu", m_ctr); return *this;}
idxcounter& idxcounter::operator --() {--m_ctr;sprintf(m_buf, "%lu", m_ctr); return *this;}
long unsigned int idxcounter::getInt(void) const {return m_ctr;}
const char* idxcounter::getStr(void) const {return m_buf;}
int idxcounter::getStrSize(void) const {return strlen(m_buf);}

