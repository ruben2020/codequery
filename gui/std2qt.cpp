
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


