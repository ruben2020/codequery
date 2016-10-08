
/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

// Cscope database header

#ifndef CSDBHEADER_H
#define CSDBHEADER_H

#include "small_lib.h"

class csdbheader
{

public:

csdbheader();
csdbheader(tStr hdr);

void set_header(tStr hdr);
void set_header(const char* hdr);
bool parse(void);
long int get_version(void);
tStr get_base_path(void);
tVecStr get_param_list(void);
long int get_trailer_start(void);
void print_contents(void);

tStr m_header;
long int m_csdbver;
tStr m_base_path;
tVecStr m_param_list;
long int m_trailer_start;


}; // class csdbheader



#endif // CSDBHEADER_H




