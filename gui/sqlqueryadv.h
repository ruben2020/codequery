
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef SQLQUERYADV_H_CQ
#define SQLQUERYADV_H_CQ

#include "sqlquery.h"

class QString;

// Advanced version of sqlquery to be MPL'ed

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



