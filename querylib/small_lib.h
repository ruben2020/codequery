
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


#ifndef SMALL_LIB_H_CQ
#define SMALL_LIB_H_CQ

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#if defined(USE_QT5)||defined(USE_QT4)
#include <QString>
#include <QStringList>
#endif


#ifdef _WIN32
#define DIRSEP '\\'
#else
#define DIRSEP '/'
#endif

// Get number of elements of a fixed-sized array
#define DIM(x)   (sizeof( x ) / sizeof( x [0] ) )

#if defined(USE_QT5)||defined(USE_QT4)
typedef QString tStr;
typedef QString::iterator tStrIter;
typedef QStringList tVecStr;
#else // only STL
typedef std::string tStr;
typedef std::string::iterator tStrIter;
typedef std::vector<std::string> tVecStr;
#endif

#if defined(USE_QT5) // use Qt5's QString
#define C_STR(x) toLatin1(x).data(x)
#define STRISEMPTY(x) isEmpty(x)
#define STRTOLOWER(x,y) x = y.toLower()

#elif defined(USE_QT4) // use Qt4's QString
#define C_STR(x) toAscii(x).data(x)
#define STRISEMPTY(x) isEmpty(x)
#define STRTOLOWER(x,y) x = y.toLower()

#else // use std::string
#define C_STR(x) c_str(x)
#define STRISEMPTY(x) empty(x)
#define STRTOLOWER(x,y) x = y; \
                        std::transform(x.begin(), x.end(), x.begin(), ::tolower)
#endif


bool check_fileExists(const char *fn);
bool isAbsolutePath(tStr fp);
bool strrevcmp(tStr str, tStr cmpstr);
char* get_last_part(char* str, int c);
std::vector<std::string> splitstr(const char* inpstr, const char delim);

// replace char o with char r for every part of the string
// from iterator i1 to iterator i2, excluding i2
template<class T>
long replacechar(T i1, T i2, const char o, const char r)
{
	long count = 0;
	for(T i = i1; i != i2; ++i)
	{
		if (*i == o)
		{
			*i = r;
			count++;
		}
	}
	return count;
}

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
long m_size;

public:
tempbuf(long n);
~tempbuf(); // the reason why this class was written
char* operator() ();
char* get(void);
char  operator[] (long i);
long size(void) const;
bool isempty(void) const;
void clear(void);
char* reinit(long n);
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

#endif //SMALL_LIB_H_CQ

