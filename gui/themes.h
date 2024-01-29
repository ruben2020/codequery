
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef THEMES_H_CQ
#define THEMES_H_CQ

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
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
	static void setKeywords(int lang, ScintillaEdit* lexer);
	static long QC2SC(QColor colour);

private:
	static void setThemeStyle(ScintillaEdit* lexer, lexstyle *lxstyle, int lxstylesize, QFont& font1);
};


#endif // THEMES_H_CQ

