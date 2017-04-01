
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef STD2QT_H_CQ
#define STD2QT_H_CQ

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

#endif //STD2QT_H_CQ

