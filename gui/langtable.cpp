
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <QString>
#include <QStringList>

#include "small_lib.h"
#include "langtable.h"

#define BASE_LANG_PATH ":/trans/"


typedef struct
{
	const char* langName;
	const char* langFile;
}langTableType;

static const langTableType langTable[]=
{
#include "tslist.txt"
};

QStringList langtable::getLangNameList(void)
{
	QStringList lst;
	for(int i=0; i<DIM(langTable); i++)
	{
		lst << QString::fromLatin1(langTable[i].langName);
	}
	lst.sort();
	return lst;
}

QString langtable::getLangFilePath(const QString& lang)
{
	QString langfp = BASE_LANG_PATH;
	QString langfn = "codequery_en";
	for (int i=0; i<DIM(langTable); i++)
	{
		if (lang.compare(QString::fromLatin1(langTable[i].langName)) == 0)
		{
			langfn = langTable[i].langFile;
			break;
		}
	}
	langfp += langfn;
	return langfp;
}


