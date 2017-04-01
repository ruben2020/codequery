
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


#ifndef THEMES_H_CQ
#define THEMES_H_CQ

#ifdef USE_QT5
#include <QtWidgets>
#else
#include <QtGui>
#endif

class ScintillaEdit;

typedef struct
{
	int styleid;
	const char *fgcolor;
	const char *bgcolor;
	int fontstyle;
}lexstyle;

class themes
{
public:
	static QStringList getThemesList(void);
	static void setTheme(const QString& theme, int lang, ScintillaEdit* lexer, const QFont& fontt, QColor& curlinebgcolor, QColor& linenumbgcolor);
	static long QC2SC(QColor colour);

private:
	static void setThemeStyle(ScintillaEdit* lexer, lexstyle *lxstyle, int lxstylesize, QFont& font1);
};


#endif // THEMES_H_CQ

