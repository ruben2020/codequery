
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


// Cscope database header


#include "csdbheader.h"
#include <stdlib.h>


csdbheader::csdbheader()
:m_csdbver(0)
,m_trailer_start(0)
{

}

csdbheader::csdbheader(tStr hdr)
:m_csdbver(0)
,m_trailer_start(0)
{
m_header = hdr;
}

void csdbheader::set_header(tStr hdr)
{
m_header = hdr;
}

void csdbheader::set_header(const char* hdr)
{
m_header = hdr;
}

bool csdbheader::parse(void)
{

size_t pos;

m_param_list.clear();
if (m_header.empty()) return false;

// Header should be at least 19 bytes long and starts with "cscope "
if ((m_header.length() <19)||(m_header.compare(0, strlen("cscope "), "cscope ") != 0))
	{return false;}

// Get the cscope version used to build the database
// Assuming it is 2 digits always
m_csdbver = strtol(m_header.substr(7, 2).c_str(), NULL, 10);

// Get the trailer start position
pos = m_header.find_last_of((char) ' ');
m_trailer_start = strtol(m_header.substr(pos + 1).c_str(), NULL, 10);

// Get rid of "cscope nn " and trailer start pos
m_header = m_header.substr(10, pos - 10);

// Get the first position of a param
pos = m_header.find((const char*) " -");

// if no params, then whole string is db path
if (pos == std::string::npos)
{
	m_base_path = m_header.substr(0);

	// Trim db path
	pos = m_base_path.find_last_not_of((char) ' ') + 1;
	m_base_path = m_base_path.substr(0, pos);

	// If the last char of base path is \", remove it
	// Cannot remember why I needed to do this
	if (*(m_base_path.rbegin()) == '\"') m_base_path.erase(m_base_path.end() - 1);
	return true;
}

// Get db path
m_base_path = m_header.substr(0, pos);

// Get rid of db path
m_header = m_header.substr(pos);

// Trim db path
pos = m_base_path.find_last_not_of((char) ' ') + 1;
m_base_path = m_base_path.substr(0, pos);

// If the last char of base path is \", remove it
// Cannot remember why I needed to do this
if (*(m_base_path.rbegin()) == '\"') m_base_path.erase(m_base_path.end() - 1);

// Prepare a list of params
pos = 0;
tStr s;
while ((pos != std::string::npos) && (pos < m_header.length()))
{
	pos = m_header.find((char) '-', pos);
	if (pos != std::string::npos)
	{
		pos++;
		s = m_header.at(pos);
		m_param_list.push_back(s);
	}
}

return true;

}

long int csdbheader::get_version(void)
{
return m_csdbver;
}

tStr csdbheader::get_base_path(void)
{
return m_base_path;
}

tVecStr csdbheader::get_param_list(void)
{
return m_param_list;
}

long int csdbheader::get_trailer_start(void)
{
return m_trailer_start;
}

void csdbheader::print_contents(void)
{
	unsigned int i;
	printf("Ver= %lu\nPath= \"%s\"\nTrailer start= %lu\nParam list= ", m_csdbver, m_base_path.c_str(), m_trailer_start);
	for (i=0; i< m_param_list.size(); i++)
	{
		printf("\"%s\", ", m_param_list[i].c_str());
	}
	printf("\n\n");
}




