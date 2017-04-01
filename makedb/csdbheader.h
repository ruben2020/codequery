
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




