
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

#include <QString>
#include <QStringList>

#include "small_lib.h"
#include "langtable.h"

#define BASE_LANG_PATH ":/mainwindow/build/gui/"


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


