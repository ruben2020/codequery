
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

#ifndef _STD2QT_H
#define _STD2QT_H

#include <QString>
#include <QStringList>

#include "small_lib.h"

inline QString str2qt(const tStr& inp)
#ifdef CQ_NO_QTSQL
	{return QString(inp.c_str());}
#else
	{return QString::fromStdString(inp);}
#endif

QStringList strLst2qt(const tVecStr& inpLst);

inline tStr qt2str(const QString& inp)
#ifdef CQ_NO_QTSQL
	{return tStr((const char*) inp.toAscii().data());}
#else
	{return inp.toStdString();}
#endif

tVecStr qt2strLst(const QStringList& inpLst);

#endif //_STD2QT_H

