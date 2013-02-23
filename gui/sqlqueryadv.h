
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

#ifndef _SQLQUERYADV_H
#define _SQLQUERYADV_H

#include "sqlquery.h"

class QString;

// Advanced version of sqlquery to be GPL'ed

class sqlqueryadv : public sqlquery
{

public:
	sqlqueryadv();
	~sqlqueryadv();

bool search_funcgraph(QString searchstr, bool exactmatch, QString& xmlout, QString& dotout);

};

#endif //_SQLQUERYADV_H



