
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

#include "ScintillaEdit.h"

#include "small_lib.h"
#include "fileviewer.h"
#include "themes.h"

#ifdef USE_QT5
#define QT45_TOASCII(x) toLatin1(x)
#else
#define QT45_TOASCII(x) toAscii(x)
#endif

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

// Keywords lists below taken from langs.model.xml of Notepad++

const char* python_keywords =
"and as assert break class continue def "
"del elif else except exec False finally "
"for from global if import in is lambda "
"None not or pass print raise return "
"True try while with yield async await";

const char* ruby_keywords =
"ARGF ARGV BEGIN END ENV FALSE DATA NIL "
"RUBY_PATCHLEVEL RUBY_PLATFORM RUBY_RELEASE_DATE "
"RUBY_VERSION PLATFORM RELEASE_DATE STDERR STDIN "
"STDOUT TOPLEVEL_BINDING TRUE __ENCODING__ "
"__END__ __FILE__ __LINE__ alias and begin break "
"case class def defined? do else elsif end ensure "
"false for if in module next nil not or redo rescue "
"retry return self super then true undef "
"unless until when while yield";

const char* js_keywords =
"abstract async await boolean break byte case catch "
"char class const continue debugger default delete do "
"double else enum export extends final finally float "
"for from function goto if implements import in instanceof "
"int interface let long native new null of package private "
"protected public return short static super switch synchronized "
"this throw throws transient try typeof var void "
"volatile while with true false prototype yield";

const char* js_types =
"Array Date eval hasOwnProperty Infinity isFinite isNaN "
"isPrototypeOf Math NaN Number Object prototype "
"String toString undefined valueOf";

const char* java_keywords =
"instanceof assert if else switch case default break goto "
"return for while do continue new throw throws try catch "
"finally this super extends implements import true false null";

const char* java_types =
"package transient strictfp void char short int long double "
"float const static volatile byte boolean class interface "
"native private protected public final abstract synchronized enum";

const char* cpp_keywords =
"alignof and and_eq bitand bitor break case catch compl const_cast "
"continue default delete do dynamic_cast else false for goto if "
"namespace new not not_eq nullptr operator or or_eq reinterpret_cast "
"return sizeof static_assert static_cast switch this "
"throw true try typedef typeid using while xor xor_eq NULL";

const char* cpp_types =
"alignas asm auto bool char char16_t char32_t class clock_t "
"const constexpr decltype double enum explicit export extern "
"final float friend inline int int8_t int16_t int32_t int64_t "
"int_fast8_t int_fast16_t int_fast32_t int_fast64_t intmax_t "
"intptr_t long mutable noexcept override private protected "
"ptrdiff_t public register short signed size_t ssize_t static "
"struct template thread_local time_t typename uint8_t uint16_t "
"uint32_t uint64_t uint_fast8_t uint_fast16_t uint_fast32_t "
"uint_fast64_t uintmax_t uintptr_t union "
"unsigned virtual void volatile wchar_t";

const char* cpp_docwords =
"a addindex addtogroup anchor arg attention author authors b brief "
"bug c callergraph callgraph category cite class code cond "
"copybrief copydetails copydoc copyright date def defgroup deprecated "
"details diafile dir docbookonly dontinclude dot dotfile e else elseif "
"em endcode endcond enddocbookonly enddot endhtmlonly endif endinternal "
"endlatexonly endlink endmanonly endmsc endparblock endrtfonly "
"endsecreflist enduml endverbatim endxmlonly enum example exception "
"extends f$ f[ f] file fn f{ f} headerfile hidecallergraph hidecallgraph "
"hideinitializer htmlinclude htmlonly idlexcept if ifnot image "
"implements include includelineno ingroup interface internal invariant "
"latexinclude latexonly li line link mainpage manonly memberof msc mscfile "
"n name namespace nosubgrouping note overload p package page par paragraph "
"param parblock post pre private privatesection property protected "
"protectedsection protocol public publicsection pure ref refitem related "
"relatedalso relates relatesalso remark remarks result return returns "
"retval rtfonly sa secreflist section see short showinitializer since "
"skip skipline snippet startuml struct subpage subsection subsubsection "
"tableofcontents test throw throws todo tparam typedef union until var "
"verbatim verbinclude version vhdlflow warning weakgroup xmlonly xrefitem";

const char* go_keywords = "break default func interface select "
"case defer go map struct chan else goto package switch "
"const fallthrough if range type continue for import return var "
"append cap close complex copy delete imag len "
"make new panic print println real recover";

const char* go_types = "true false iota nil "
"bool byte complex64 complex128 error float32 float64 "
"int int8 int16 int32 int64 rune string "
"uint uint8 uint16 uint32 uint64 uintptr";


QStringList themes::getThemesList(void)
{
	QStringList lst;
	for(int i=0; i<NUM_OF_THEMES; i++)
	{
		lst << QString::fromLatin1(themelist[i]);
	}
	return lst;
}

void themes::setKeywords(int lang, ScintillaEdit* lexer)
{
	int i;
	for (i=1; i <= 2; i++) lexer->setKeyWords(i, " ");
	switch(lang)
	{
		case enHighlightPython:
		lexer->setKeyWords(0, python_keywords);
		break;

		case enHighlightJava:
		lexer->setKeyWords(0, java_keywords);
		lexer->setKeyWords(1, java_types);
		break;

		case enHighlightRuby:
		lexer->setKeyWords(0, ruby_keywords);
		break;

		case enHighlightJavascript:
		lexer->setKeyWords(0, js_keywords);
		lexer->setKeyWords(1, js_types);
		break;

		case enHighlightGo:
		lexer->setKeyWords(0, go_keywords);
		lexer->setKeyWords(1, go_types);
		break;

		case enHighlightCPP:
			// fall through
		default:
		lexer->setKeyWords(0, cpp_keywords);
		lexer->setKeyWords(1, cpp_types);
		lexer->setKeyWords(2, cpp_docwords);
		break;
	}

}

long themes::QC2SC(QColor colour)
{
	long retval;
	retval  =  (long) colour.red();
	retval |= ((long) colour.green()) << 8;
	retval |= ((long) colour.blue())  << 16;
	return  retval;
}

void themes::setTheme(const QString& theme, int lang, ScintillaEdit* lexer, const QFont& fontt, QColor& curlinebgcolor, QColor& linenumbgcolor)
{
	langstyle *lngstyle = NULL;
	lexstyle *lxstyle = NULL;
	lexstyle *globallxstyle = NULL;
	int i=0;
	int lxstylesize=0;
	int globallxstylesize=0;
	long defbgcolor;
	long deffgcolor;
	QFont font1 = fontt;
	//font1.setFixedPitch(true);
	font1.setBold(false);
	font1.setItalic(false);
	for (i=0; i < 4; i++) lexer->setKeyWords(i, "");
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
		//lngstyle = (langstyle *) javascriptstyle;
		break;

		default:
		lngstyle = (langstyle *) cppstyle;
		break;
	}
	i = 0;
	while (lngstyle[i].lexstylesize > 0)
	{
		if (theme.compare(QString(lngstyle[i].themename)) == 0)
		{
			lxstyle = (lexstyle *) lngstyle[i].lexstyletable;
			globallxstyle = (lexstyle *) lngstyle[i].globallexstyletable;
			lxstylesize = lngstyle[i].lexstylesize;
			globallxstylesize = lngstyle[i].globallexstylesize;
			defbgcolor = QC2SC(QColor(QString("#").append(QString(lngstyle[i].defaultbgcolor))));
			deffgcolor = QC2SC(QColor(QString("#").append(QString(lngstyle[i].defaultfgcolor))));
			lexer->styleSetBack(  STYLE_DEFAULT, defbgcolor);
			lexer->styleSetFore(  STYLE_DEFAULT, deffgcolor);
			lexer->styleSetFont(  STYLE_DEFAULT, font1.family().QT45_TOASCII().data());
			lexer->styleSetBold(  STYLE_DEFAULT, false);
			lexer->styleSetItalic(STYLE_DEFAULT, false);
			lexer->styleClearAll();
			lexer->setCaretFore(deffgcolor);
			break;
		}
		i++;
	}
	setThemeStyle(lexer, globallxstyle, globallxstylesize, font1);
	setThemeStyle(lexer, lxstyle      , lxstylesize      , font1);
	curlinebgcolor = QColor(QString("#").append(QString(lngstyle[i].currentlinebgcolor)));
	linenumbgcolor = QColor(QString("#").append(QString(lngstyle[i].linenumfgcolor)));
}

void themes::setThemeStyle(ScintillaEdit* lexer, lexstyle *lxstyle, int lxstylesize, QFont& font1)
{
	int i;
	if (lxstyle != NULL)
	for(i=0; i<lxstylesize; i++)
	{
		if (
			(lxstyle[i].styleid == 0) ||
			(lxstyle[i].styleid == STYLE_DEFAULT))
				continue;
		lexer->styleSetBack(lxstyle[i].styleid, QC2SC(QColor(QString("#").append(QString(lxstyle[i].bgcolor)))));
		lexer->styleSetFore(lxstyle[i].styleid, QC2SC(QColor(QString("#").append(QString(lxstyle[i].fgcolor)))));
		lexer->styleSetFont(lxstyle[i].styleid, font1.family().QT45_TOASCII().data());
		switch(lxstyle[i].fontstyle)
		{
			case 1:
				lexer->styleSetBold(lxstyle[i].styleid, true);
				lexer->styleSetItalic(lxstyle[i].styleid, false);
				break;

			case 2:
				lexer->styleSetBold(lxstyle[i].styleid, false);
				lexer->styleSetItalic(lxstyle[i].styleid, true);
				break;

			case 3:
				lexer->styleSetBold(lxstyle[i].styleid, true);
				lexer->styleSetItalic(lxstyle[i].styleid, true);
				break;

			default:
				lexer->styleSetBold(lxstyle[i].styleid, false);
				lexer->styleSetItalic(lxstyle[i].styleid, false);
				break;

		}
	}
}

