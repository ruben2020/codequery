
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
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QtGui>

 #include "highlighter.h"

 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent),
       m_intAddlRulesMode(0)
 {
     HighlightingRule rule;

     functionFormat.setFontItalic(true);
     functionFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
     rule.format = functionFormat;
     highlightingRules.append(rule);

     keywordFormat.setForeground(Qt::darkBlue);
     keywordFormat.setFontWeight(QFont::Bold);
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
         highlightingRules.append(rule);
     }

     literalFormat.setFontWeight(QFont::Bold);
     literalFormat.setForeground(Qt::darkYellow);
     rule.pattern = QRegExp("\\b[0-9]+\\b");
     rule.format = literalFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::darkGreen);

     quotationFormat.setForeground(Qt::red);
     singlequotationFormat.setForeground(Qt::red);
     singleLineCommentFormat.setForeground(Qt::darkGreen);

     cppFormat.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("^[\t ]*#.+");
     rule.format = cppFormat;
     highlightingRules.append(rule);

     /*
     quotationFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("\".*\"");
     rule.format = quotationFormat;
     highlightingRules.append(rule);
     singlequotationFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("\'.*\'");
     rule.format = singlequotationFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("//[^\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules2.append(rule);
     */

     commentStartExpression = QRegExp("/\\*");
     commentEndExpression = QRegExp("\\*/");
 }

 void Highlighter::highlightBlock(const QString &text)
 {
    foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         int length;
         while (index >= 0) {
             length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
    myAdditionalRules(text);
 }

 void Highlighter::myAdditionalRules(const QString &text)
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
                   if ((c2 == '\n')||(c2 == '\r'))
                       {
                           mode = 0;
                           endindex = i;
                           setFormat(startindex, endindex - startindex + 1, singleLineCommentFormat);
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

// Let's test some comment formats below:
//****** // /*****/
#if (0)

#define BLURB  xcvxcxcvxcv \
          sdfsdfsdfsdfsdfsd \
          sdfsdfsdfs

#define BLURB  xcvxcxcvxcv \   //hello
          sdfsdfsdfsdfsdfsd \  /******/
          sdfsdfsdfs


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

