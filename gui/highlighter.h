 
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

 #ifndef HIGHLIGHTER_H
 #define HIGHLIGHTER_H

 #include <QSyntaxHighlighter>

 #include <QHash>
 #include <QTextCharFormat>

 class QTextDocument;

 enum enHighlightLang
 {
     enHighlightCPP = 0,
     enHighlightJava,
     enHighlightPython
 };
 
 class Highlighter : public QSyntaxHighlighter
 {
     Q_OBJECT

 public:
     Highlighter(QTextDocument *parent = 0);
     int m_intAddlRulesMode;
     int m_intLanguage;

 protected:
     void highlightBlock(const QString &text);

 private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> cHighlightingRules;
     QVector<HighlightingRule> javaHighlightingRules;
     QVector<HighlightingRule> pyHighlightingRules;

     QTextCharFormat keywordFormat;
     QTextCharFormat literalFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat singlequotationFormat;
     QTextCharFormat cppFormat;
     QTextCharFormat functionFormat;

     void cAdditionalRules(const QString &text);
     void pyAdditionalRules(const QString &text);
     inline void execHighlightRule(const HighlightingRule &rule, const QString &text);
     void setup_CPP(void);
     void setup_Java(void);
     void setup_Python(void);
 };

 #endif

