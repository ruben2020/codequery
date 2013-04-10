
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

 /****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ****************************************************************************/

 #include <QtGui>

 #include "highlighter.h"

 void Highlighter::setup_CPP(void)
 {
     HighlightingRule rule;

     rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
     rule.format = functionFormat;
     cHighlightingRules.append(rule);

     QStringList keywordPatterns;
     keywordPatterns  << "\\bauto\\b" << "\\bconst\\b" << "\\bdouble\\b" << "\\bfloat\\b"
	<< "\\bint\\b" << "\\bshort\\b" << "\\bstruct\\b" << "\\bunsigned\\b"
	<< "\\bbreak\\b" << "\\bcontinue\\b" << "\\belse\\b" << "\\bfor\\b"
	<< "\\blong\\b" << "\\bsigned\\b" << "\\bswitch\\b" << "\\bvoid\\b"
	<< "\\bcase\\b" << "\\bdefault\\b" << "\\benum\\b" << "\\bgoto\\b"
	<< "\\bregister\\b" << "\\bsizeof\\b" << "\\btypedef\\b" << "\\bvolatile\\b"
	<< "\\bchar\\b" << "\\bdo\\b" << "\\bextern\\b" << "\\bif\\b" << "\\breturn\\b"
	<< "\\bstatic\\b" << "\\bunion\\b" << "\\bwhile\\b"

	<< "\\basm\\b" << "\\bdynamic_cast\\b" << "\\bnamespace\\b" << "\\breinterpret_cast\\b"
	<< "\\btry\\b" << "\\bbool\\b" << "\\bexplicit\\b" << "\\bnew\\b"
	<< "\\bstatic_cast\\b" << "\\btypeid\\b" << "\\bcatch\\b" << "\\bfalse\\b"
	<< "\\boperator\\b" << "\\btemplate\\b" << "\\btypename\\b" << "\\bclass\\b"
	<< "\\bfriend\\b" << "\\bprivate\\b" << "\\bthis\\b" << "\\busing\\b"
	<< "\\bconst_cast\\b" << "\\binline\\b" << "\\bpublic\\b" << "\\bthrow\\b"
	<< "\\bvirtual\\b" << "\\bdelete\\b" << "\\bmutable\\b" << "\\bprotected\\b"
	<< "\\btrue\\b" << "\\bwchar_t\\b"

	<< "\\band\\b" << "\\bbitand\\b" << "\\bcompl\\b" << "\\bnot_eq\\b"
	<< "\\bor_eq\\b" << "\\bxor_eq\\b" << "\\band_eq\\b" << "\\bbitor\\b"
	<< "\\bnot\\b" << "\\bor\\b" << "\\bxor\\b";


     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         cHighlightingRules.append(rule);
     }

     rule.pattern = QRegExp("\\b[0-9]+\\b");
     rule.format = literalFormat;
     cHighlightingRules.append(rule);

     rule.pattern = QRegExp("^[\t ]*#.+");
     rule.format = cppFormat;
     cHighlightingRules.append(rule);

     /*
     rule.pattern = QRegExp("\".*\"");
     rule.format = quotationFormat;
     highlightingRules.append(rule);
     rule.pattern = QRegExp("\'.*\'");
     rule.format = singlequotationFormat;
     highlightingRules.append(rule);

     rule.pattern = QRegExp("//[^\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules2.append(rule);
     */
 }

 void Highlighter::setup_Python(void)
 {
     HighlightingRule rule;

     rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
     rule.format = functionFormat;
     pyHighlightingRules.append(rule);

     QStringList keywordPatterns;
     keywordPatterns  << "\\band\\b" << "\\bassert\\b" << "\\bas\\b" << "\\bbreak\\b"
	 << "\\bclass\\b" << "\\bcontinue\\b" << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b"
	 << "\\belse\\b" << "\\bexcept\\b" << "\\bexec\\b" << "\\bfinally\\b" << "\\bfor\\b"
	 << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b" << "\\bin\\b"
	 << "\\bis\\b" << "\\blambda\\b" << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b"
	 << "\\bprint\\b" << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b"
	 << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";


     foreach (const QString &pattern, keywordPatterns) {
         rule.pattern = QRegExp(pattern);
         rule.format = keywordFormat;
         pyHighlightingRules.append(rule);
     }

     rule.pattern = QRegExp("\\b[0-9]+\\b");
     rule.format = literalFormat;
     pyHighlightingRules.append(rule);

 }
 
 
 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent),
       m_intAddlRulesMode(0),
	   m_intLanguage(enHighlightCPP)
 {
     // TODO: Syntax highlight themes
     functionFormat.setFontItalic(true);
     functionFormat.setForeground(Qt::blue);
     keywordFormat.setForeground(Qt::darkBlue);
     keywordFormat.setFontWeight(QFont::Bold);
     literalFormat.setFontWeight(QFont::Bold);
     literalFormat.setForeground(Qt::darkYellow);
     multiLineCommentFormat.setForeground(Qt::darkGreen);
     singleLineCommentFormat.setForeground(Qt::darkGreen);
     quotationFormat.setForeground(Qt::red);
     singlequotationFormat.setForeground(Qt::red);
     cppFormat.setForeground(Qt::darkMagenta);
	 
     setup_CPP();
     setup_Python();
 }

 void Highlighter::highlightBlock(const QString &text)
 {
    if (m_intLanguage == enHighlightCPP)
	{
	     foreach (const HighlightingRule &rule, cHighlightingRules) {
             execHighlightRule(rule, text);
         }
         cAdditionalRules(text);
    }
    else if (m_intLanguage == enHighlightPython)
	{
	     foreach (const HighlightingRule &rule, pyHighlightingRules) {
             execHighlightRule(rule, text);
         }
         pyAdditionalRules(text);
    }
 }

 void Highlighter::execHighlightRule(const HighlightingRule &rule, const QString &text)
 {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         int length;
         while (index >= 0) {
             length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
 }
 
 void Highlighter::cAdditionalRules(const QString &text)
 {
     int mode = m_intAddlRulesMode; //0=nothing, 1=CPPcomment, 2=dblQuote, 3=singleQuote, 4=Ccomment
     int startindex=0;
     int endindex=0;
     static char c1 = ' ';
     static char c2 = ' ';
     uint i = 0;
     uint maxi = text.size() - 1;
     if (maxi == -1) return;
     if (mode == -1) {mode = 0; c1 = ' '; c2 = ' ';}
     while((i <= maxi)&&(i >= 0))
     {
        c1 = c2;
        c2 = text[i].toAscii();
        switch (mode)
        {
           case 0:
                   if ((c1 == '/')&&(c2 == '/'))
                      {mode = 1; startindex = i-1; c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if ((c1 == '/')&&(c2 == '*'))
                      {mode = 4; startindex = i-1; c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if (c1 == '"')  {mode = 2; startindex = i-1;}
                   else if (c1 == '\'') {mode = 3; startindex = i-1;}
                   break;
           case 1:
                   if ((c2 == '\n')||(c2 == '\r')||(i == maxi))
                       {
                           mode = 0;
                           endindex = i;
                           setFormat(startindex, endindex - startindex + 1, singleLineCommentFormat);
                           c2 = ' '; //if new line is reached, reset scanners
                       }
                   break;
           case 2:
                   if ((c1 == '\\')&&(c2 == '\\')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if ((c1 == '\\')&&(c2 == '"')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if (c1 == '"')  {mode = 0; endindex = i-1;
                        setFormat(startindex, endindex - startindex + 1, quotationFormat);}
                   break;
           case 3:
                   if ((c1 == '\\')&&(c2 == '\\')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if ((c1 == '\\')&&(c2 == '\'')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if (c1 == '\'')  {mode = 0; endindex = i-1;
                        setFormat(startindex, endindex - startindex + 1, singlequotationFormat);}
                   break;
           case 4:
                   if ((c1 == '*')&&(c2 == '/'))
                       {
                           mode = 0;
                           endindex = i;
                           setFormat(startindex, endindex - startindex + 1, multiLineCommentFormat);
                           c1=c2; c2 = text[i+1].toAscii(); i++;
                       }
                   break;
        }
        i++;
     }
     endindex = i-1;
     switch (mode)
     {
       case 1:
               setFormat(startindex, endindex - startindex + 1, singleLineCommentFormat);
               mode = 0;
               break;
       case 2:
               setFormat(startindex, endindex - startindex + 1, quotationFormat);
               break;
       case 3:
               setFormat(startindex, endindex - startindex + 1, singlequotationFormat);
               break;
       case 4:
               setFormat(startindex, endindex - startindex + 1, multiLineCommentFormat);
               break;
       default:;
     }
     m_intAddlRulesMode = mode;
 }

 void Highlighter::pyAdditionalRules(const QString &text)
 {
     int mode = m_intAddlRulesMode; //0=nothing, 1=comment, 2=dblQuote, 3=singleQuote, 4=tripDblQuote, 5=tripSingleQuote
     int startindex=0;
     int endindex=0;
     static char cminus1 = ' ';
     static char c0 = ' ';
     static char c1 = ' ';
     static char c2 = ' ';
     uint i = 0;
     uint maxi = text.size() - 1;
     if (maxi == -1) return;
     if (mode == -1) {mode = 0; c0 = ' '; c1 = ' '; c2 = ' '; cminus1 = ' ';}
     while((i <= maxi)&&(i >= 0))
     {
        cminus1 = c0;
        c0 = c1;
        c1 = c2;
        c2 = text[i].toAscii();
        switch (mode)
        {
           case 0:
                   if (c1 == '#') {mode = 1; startindex = i-1; }
                   else if ((cminus1 == '"') &&(c0 == '"') &&(c1 == '"'))  {mode = 4; startindex = i-3;}
                   else if ((cminus1 == '\'')&&(c0 == '\'')&&(c1 == '\'')) {mode = 5; startindex = i-3;}
                   else if (c1 == '"')  {mode = 2; startindex = i-1;}
                   else if (c1 == '\'') {mode = 3; startindex = i-1;}
                   break;
           case 1:
                   if ((c2 == '\n')||(c2 == '\r')||(i == maxi))
                       {
                           mode = 0;
                           endindex = i;
                           setFormat(startindex, endindex - startindex + 1, singleLineCommentFormat);
                           c2 = ' '; //if new line is reached, reset scanners
                       }
                   break;
           case 2:
                   if ((c1 == '\\')&&(c2 == '\\')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if ((c1 == '\\')&&(c2 == '"')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if (c1 == '"')  {mode = 0; endindex = i-1;
                        setFormat(startindex, endindex - startindex + 1, quotationFormat);}
                   break;
           case 3:
                   if ((c1 == '\\')&&(c2 == '\\')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if ((c1 == '\\')&&(c2 == '\'')) {c1=c2; c2 = text[i+1].toAscii(); i++;}
                   else if (c1 == '\'')  {mode = 0; endindex = i-1;
                        setFormat(startindex, endindex - startindex + 1, singlequotationFormat);}
                   break;
           case 4:
                   if ((cminus1 == '"')&&(c0 == '"')&&(c1 == '"'))
                   {
                      mode = 0;
                      endindex = i-1;
                      setFormat(startindex, endindex - startindex + 1, quotationFormat);
                   }
                   break;
           case 5:
                   if ((cminus1 == '\'')&&(c0 == '\'')&&(c1 == '\''))
                   {
                      mode = 0;
                      endindex = i-1;
                      setFormat(startindex, endindex - startindex + 1, quotationFormat);
                   }
                   break;
        }
        i++;
     }
     endindex = i-1;
     switch (mode)
     {
       case 1:
               setFormat(startindex, endindex - startindex + 1, singleLineCommentFormat);
               mode = 0;
               break;
       case 2:
               setFormat(startindex, endindex - startindex + 1, quotationFormat);
               break;
       case 3:
               setFormat(startindex, endindex - startindex + 1, singlequotationFormat);
               break;
       case 4:
               setFormat(startindex, endindex - startindex + 1, quotationFormat);
               break;
       case 5:
               setFormat(startindex, endindex - startindex + 1, singlequotationFormat);
               break;
       default:;
     }
     m_intAddlRulesMode = mode;
 }

 
// Let's test some comment formats below:
//****** // /*****/
#if (0)

#define BLURB  xcvxcxcvxcv \
          sdfsdfsdfsdfsdfsd \
          sdfsdfsdfs

#define BLURB  xcvxcxcvxcv \   //hello
          sdfsdfsdfsdfsdfsd \  /******/
          sdfsdfsdfs

// Another test "hello"

void test(void)
{
   QString str;
   QChar cas;
   str = "/*";
   str = "\"/*\"";
   cas = '\'';
   /*//******* // *******
   ***********//***/
}
#endif

