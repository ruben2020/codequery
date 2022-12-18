
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef FILEVIEWER_H_CQ
#define FILEVIEWER_H_CQ

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "sqlquery.h"
Q_DECLARE_METATYPE(sqlqueryresultlist*)


class mainwindow;
class ScintillaEdit;


class filedata
{
	public:
	QString filename;
	QString linenum;
	int fileid;
	
	filedata();
	filedata(const QString& fn, const QString& ln, const int& fi);
	filedata(const filedata& fd);
	bool compare(const filedata& fd);
	bool compareFilePathOnly(const filedata& fd);
	bool compareFileNameOnly(const filedata& fd);
	filedata& operator=(const filedata& fd);
};


enum langtypes
{
	enHighlightNone = 0,
	enHighlightCPP,
	enHighlightPython,
	enHighlightJava,
	enHighlightRuby,
	enHighlightGo,
	enHighlightJavascript
};

class fileviewer : public QObject
{
  Q_OBJECT

public:
QPushButton *m_pushButtonPrev;
QPushButton *m_pushButtonNext;
QPushButton *m_pushButtonOpenInEditor;
QPushButton *m_pushButtonPaste;
QPushButton *m_pushButtonGoToLine;
QPushButton *m_pushButtonTextShrink;
QPushButton *m_pushButtonTextEnlarge;
QCheckBox   *m_checkBoxSymbolOnly;
QLineEdit *m_labelFilePath;
ScintillaEdit *m_textEditSource;
QListWidget *m_listWidgetFunc;
QComboBox *m_comboBoxFuncListSort;
QString m_externalEditorPath;
QFont m_textEditSourceFont;
int m_lexer;
int m_fontsize;
QString m_theme;

fileviewer(mainwindow* pmw);
~fileviewer();
void init(void);
void updateTextEdit(void);
void updateFilePathLabel(void);
void handleFileCannotBeOpenedCase(void);
QString checkFontFamily(QString fontname);

public slots:
void fileToBeOpened(QString filename, QString linenum, int fileid);
void AbleToCopy(bool copy);
void GoToLine_ButtonClick(bool checked);
void Paste_ButtonClick(bool checked);
void Prev_ButtonClick(bool checked);
void Next_ButtonClick(bool checked);
void OpenInEditor_ButtonClick(bool checked);
void TextShrink_ButtonClick(bool checked);
void TextEnlarge_ButtonClick(bool checked);
void OptionsExtEditor_Triggered(bool checked);
void fileViewSettings_Triggered(bool checked);
void clearList();
void recvDBtimestamp(QDateTime dt);
void fontSelectionTemporary(const QString &fonttxt);
void themeSelectionTemporary(const QString &themetxt);
void tabWidthSelectionTemporary(const QString &width);
void annotate(QStringList annotstrLst);
void recvFuncList(sqlqueryresultlist* reslist);
void funcItemSelected(QListWidgetItem * curitem);
void FuncListSort_indexChanged(const int& idx);
void filePathLabelTextResized();
void repaintWidget();

signals:
void searchCopiedText();
void searchCopiedTextSymbolOnly();
void requestAnnotation(QString searchstr);
void requestFuncList_filename(QString filename);
void requestFuncList_fileid(int fileid);

private:
mainwindow *mw;
QVector<filedata> m_fileDataList;
QVector<filedata>::iterator m_iter;
QDateTime m_DBtimestamp;
bool m_timestampMismatchWarned;
QStringList m_fontlist;
QString m_fonttemp;
QString m_themetemp;
QString m_themelast;
QString m_filepathlabeltextfull;
int m_currentlang;
int m_fontwidthtemp;
int m_markerhandle;
int m_markerhandle2;
long m_currentline;
long m_annotline;
sqlqueryresultlist m_funclist;

void createFontList(void);
void textSizeChange(int n);
void highlightLine(unsigned int num = 0);
void setLexer(int lang = -1);
void replaceLexer(int sclang, int lang);
void clearTextEdit(void);
void braceMatchCheck(void);
void updateFuncList(void);
void setFilePathLabelText(QString text);

};


#endif
