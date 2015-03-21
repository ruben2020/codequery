
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


#ifndef THEMES_H_CQ
#define THEMES_H_CQ

#include <QtGui>

class QsciLexer;

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
	static void setTheme(const QString& theme, int lang, QsciLexer* lexer, const QFont& fontt, QColor& curlinebgcolor, QColor& linenumbgcolor);

private:
	static void setThemeStyle(QsciLexer* lexer, lexstyle *lxstyle, int lxstylesize, QFont& font1);
};


#endif // THEMES_H_CQ

