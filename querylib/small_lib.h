
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


#ifndef _SMALL_LIB_H
#define _SMALL_LIB_H

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>


#ifdef _WIN32
#define DIRSEP '\\'
#else
#define DIRSEP '/'
#endif

// Get number of elements of a fixed-sized array
#define DIM(x)   (sizeof( x ) / sizeof( x [0] ) )

typedef std::string tStr;
typedef std::vector<std::string> tVecStr;

bool check_fileExists(const char *fn);
bool isAbsolutePath(tStr fp);
std::vector<std::string> splitstr(const char* inpstr, const char delim);
int replacechar(std::string::iterator i1, std::string::iterator i2,
			const char o, const char r);
const char* chomp(char* str);
std::string add_escape_char(std::string ori, char chr2escp, char escpchr);
std::string add_escape_char(const char* oristr, char chr2escp, char escpchr);
const char* extract_filename(const char* filepath);

class smartFILE
{
private:
FILE* m_fp;
inline void setme(FILE *fptr);

public:
smartFILE();
smartFILE(FILE *fptr);
smartFILE(const smartFILE& sfp);
~smartFILE(); // the reason why this class was written
FILE* operator() ();
bool operator ==(FILE* fptr) const;
bool operator !=(FILE* fptr) const;
bool operator ==(const smartFILE& sfp) const;
bool operator !=(const smartFILE& sfp) const;
smartFILE& operator =(FILE *fptr);
smartFILE& operator =(const smartFILE& sfp);
void assign(FILE* fptr);
void assign(const smartFILE& sfp);
FILE* get(void);
void close_file(void);
}; //class smartFILE


class tempbuf
{
private:
char* m_buffer;
unsigned int m_size;

public:
tempbuf(unsigned int n);
~tempbuf(); // the reason why this class was written
char* operator() ();
char* get(void);
char  operator[] (unsigned int i);
unsigned int size(void) const;
bool isempty(void) const;
void clear(void);
char* reinit(unsigned int n);
const char* constchar(void);
}; //class tempbuf

class idxcounter
{
private:
	long unsigned int m_ctr;
	char m_buf[20];

public:
idxcounter();
~idxcounter();
idxcounter(const idxcounter& idxc);
idxcounter& operator =(long unsigned int idx);
idxcounter& operator =(const idxcounter& idxc);
void setCounterVal(long unsigned int i);
void reset(void);
idxcounter& operator ++();
idxcounter& operator --();
long unsigned int getInt(void) const;
const char* getStr(void) const;
int getStrSize(void) const;
}; //class idxcounter

#endif //_SMALL_LIB_H

