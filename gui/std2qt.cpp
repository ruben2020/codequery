
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include "std2qt.h"


QStringList strLst2qt(const tVecStr& inpLst)
{
	QStringList res;
	unsigned int n = inpLst.size();
	for(unsigned int i=0; i < n; i++)
	{
		res << str2qt(inpLst[i]);
	}
	return res;
}


tVecStr qt2strLst(const QStringList& inpLst)
{
	tVecStr res;
	int n = inpLst.size();
	res.reserve(n);
	for(int i=0; i < n; i++)
	{
		res.push_back(qt2str(inpLst[i]));
	}
	return res;	
}


