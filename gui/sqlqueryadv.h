
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

#ifndef SQLQUERYADV_H_CQ
#define SQLQUERYADV_H_CQ

#include "sqlquery.h"

class QString;

// Advanced version of sqlquery to be GPL'ed

class sqlqueryadv : public sqlquery
{

public:
	sqlqueryadv();
	~sqlqueryadv();

bool search_funcgraph(QString searchstr, bool exactmatch, QStringList& xmlout, QStringList& dotout, int levels = 1);
bool search_classinheritgraph(QString searchstr, bool exactmatch, QStringList& xmlout, QStringList& dotout);
void unique_symnames(sqlqueryresultlist& res);
void remove_symname(sqlqueryresultlist& res, tStr name);

};

#endif //SQLQUERYADV_H_CQ



