
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

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerjavascript.h>


#include "small_lib.h"
#include "fileviewer.h"
#include "themes.h"


typedef struct
{
	const char *themename;
	const char *defaultfgcolor;
	const char *defaultbgcolor;
	const char *currentlinebgcolor;
	const char *linenumfgcolor;
	const lexstyle *lexstyletable;
	const lexstyle *globallexstyletable;
	int lexstylesize;
	int globallexstylesize;
}langstyle;


#include "themes_gen.cpp"

QStringList themes::getThemesList(void)
{
	QStringList lst;
	for(int i=0; i<NUM_OF_THEMES; i++)
	{
		lst << QString::fromLatin1(themelist[i]);
	}
	return lst;
}

void themes::setTheme(const QString& theme, int lang, QsciLexer* lexer, const QFont& fontt, QColor& curlinebgcolor, QColor& linenumbgcolor)
{
	langstyle *lngstyle = NULL;
	lexstyle *lxstyle = NULL;
	lexstyle *globallxstyle = NULL;
	int i=0;
	int lxstylesize=0;
	int globallxstylesize=0;
	QFont font1 = fontt;
	//font1.setFixedPitch(true);
	font1.setBold(false);
	font1.setItalic(false);
	switch(lang)
	{
		case enHighlightCPP:
		lngstyle = (langstyle *) cppstyle;
		break;

		case enHighlightPython:
		lngstyle = (langstyle *) pythonstyle;
		break;

		case enHighlightJava:
		lngstyle = (langstyle *) javastyle;
		break;

		case enHighlightRuby:
		lngstyle = (langstyle *) rubystyle;
		break;

		case enHighlightJavascript:
		lngstyle = (langstyle *) cppstyle;
//		lngstyle = (langstyle *) javascriptstyle;
		break;

		default:
		lngstyle = (langstyle *) cppstyle;
		break;
	}
	while (lngstyle[i].lexstylesize > 0)
	{
		if (theme.compare(QString(lngstyle[i].themename)) == 0)
		{
			lxstyle = (lexstyle *) lngstyle[i].lexstyletable;
			globallxstyle = (lexstyle *) lngstyle[i].globallexstyletable;
			lxstylesize = lngstyle[i].lexstylesize;
			globallxstylesize = lngstyle[i].globallexstylesize;
			lexer->setPaper(QColor(QString("#").append(QString(lngstyle[i].defaultbgcolor))));
			lexer->setColor(QColor(QString("#").append(QString(lngstyle[i].defaultfgcolor))));
			lexer->setFont(font1);
			break;
		}
		i++;
	}
	setThemeStyle(lexer, globallxstyle, globallxstylesize, font1);
	setThemeStyle(lexer, lxstyle      , lxstylesize      , font1);
	curlinebgcolor = QColor(QString("#").append(QString(lngstyle[i].currentlinebgcolor)));
	linenumbgcolor = QColor(QString("#").append(QString(lngstyle[i].linenumfgcolor)));
}

void themes::setThemeStyle(QsciLexer* lexer, lexstyle *lxstyle, int lxstylesize, QFont& font1)
{
	int i;
	if (lxstyle != NULL)
	for(i=0; i<lxstylesize; i++)
	{
		lexer->setPaper(QColor(QString("#").append(QString(lxstyle[i].bgcolor))), lxstyle[i].styleid);
		lexer->setColor(QColor(QString("#").append(QString(lxstyle[i].fgcolor))), lxstyle[i].styleid);
		switch(lxstyle[i].fontstyle)
		{
			case 1:
				font1.setBold(true);
				font1.setItalic(false);
				break;

			case 2:
				font1.setBold(false);
				font1.setItalic(true);
				break;

			case 3:
				font1.setBold(true);
				font1.setItalic(true);
				break;

			default:
				font1.setBold(false);
				font1.setItalic(false);
				break;

		}
		lexer->setFont(font1, lxstyle[i].styleid);
	}
}

