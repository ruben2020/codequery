
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef SEARCHHANDLER_H_CQ
#define SEARCHHANDLER_H_CQ

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "sqlquery.h"

class mainwindow;

class searchitem
{
public:
	searchitem();
	~searchitem(){}
	searchitem(const searchitem& otheritem);
	searchitem& operator=(const searchitem& otheritem);

	// return value: 0=same, 1=completely different, 2=only linenum changed
	int compare(const searchitem& otheritem);

	QString searchterm;
	QString filterterm;
	bool exactmatch;
	sqlquery::en_queryType qtype;
	int rownum;
};

class searchhandler : public QObject
{
  Q_OBJECT

public:
QPushButton *m_pushButtonOpenDB;
QComboBox *m_comboBoxDB;
QCheckBox *m_checkBoxAutoComplete;
QCheckBox *m_checkBoxExactMatch;
QPushButton *m_pushButtonSearch;
QPushButton *m_pushButtonClipSearch;
QPushButton *m_pushButtonSearchPrev;
QPushButton *m_pushButtonSearchNext;
QComboBox *m_comboBoxSearch;
QComboBox *m_comboBoxQueryType;
QPushButton *m_pushButtonGraph;
QPushButton *m_pushButtonFilesList;
QCheckBox *m_checkBoxFilter;
QComboBox *m_comboBoxFilter;
QCompleter *m_completer;
QFutureWatcher<QStringList> m_autocompFutureWatcher;
QFutureWatcher<QStringList> m_declarFutureWatcher;
QFutureWatcher<sqlqueryresultlist> m_listFuncFutureWatcher;
static bool m_grepExactMatch;
static QRegExp* m_grepRegExp;

searchhandler(mainwindow* pmw);
~searchhandler();
void init(void);
void perform_open_db(void);
void perform_search(QString searchtxt,
			bool exactmatch,
			sqlquery::en_queryType qrytyp = sqlquery::sqlresultDEFAULT,
			QString filtertxt = "",
			int selectitem = 0,
			bool updSearchMemory = true);
void updateSearchHistory(const QString& searchtxt);
void addToSearchMemory(const QString& searchtxt, const QString& filtertxt);
void goForwardInSearchMemory(void);
void goBackInSearchMemory(void);
void restoreSearchMemoryItem(void);
void retranslateUi(void);
static QStringList search_autocomplete_qt(QString searchtxt);
static QStringList search_declaration_qt(QString searchtxt);
static sqlqueryresultlist search_funclist_qt_filename(QString filename);
static sqlqueryresultlist search_funclist_qt_fileid(int fileid);
static sqlqueryresultlist doGrep(const QPair<QString, int> &fp);
static void collateGrep(sqlqueryresultlist &result,
			const sqlqueryresultlist &intermediate);

public slots:
void OpenDB_ButtonClick(bool checked);
void Search_ButtonClick(bool checked);
void PrevSearch_ButtonClick(bool checked);
void NextSearch_ButtonClick(bool checked);
void ClipSearch_ButtonClick(bool checked);
void Graph_ButtonClick(bool checked);
void FilesList_ButtonClick(bool checked);
void Search_EnterKeyPressed();
void searchTextEdited(const QString& searchtxt);
void newSearchText();
void newSearchTextSymbolOnly();
void autoCompleteStateChanged(int state);
void OpenDB_indexChanged(const int& idx);
void QueryType_indexChanged(const int& idx);
void updateListItemRowNum(const int& row);
void resultCurrentListItemSymbolName(const QString symName);
void searchDeclaration(QString searchstr);
void searchFuncList_filename(QString filename);
void searchFuncList_fileid(int fileid);
void autoCompleteFinished();
void declarSearchFinished();
void funcListSearchFinished();

signals:
void searchresults(sqlqueryresultlist resultlist, int selectitem);
void updateStatus(const QString & message, int timeout = 0);
void DBreset();
void sendDBtimestamp(QDateTime dt);
void getResultCurrentListItemSymbolName();
void searchDeclarationResultsReady(QStringList resdeclar);
void searchListFuncResultsReady(sqlqueryresultlist* res);

private:
mainwindow *mw;
static sqlquery* sq;
QString m_graphdesc;
int m_typeOfGraph; // 1 = Function Call, 2 = Class Inheritance
QStringListModel m_srchStrLstModel;
QString sqlerrormsg(sqlquery::en_filereadstatus status);
QVector<searchitem> m_searchMemoryList;
QVector<searchitem>::iterator m_iter;
bool m_autocompBusy;
bool m_declarBusy;
bool m_funcListBusy;
QString m_autocompSrchTerm;

void updateFilterHistory(QString filtertxt);
sqlqueryresultlist perform_grep(QString searchtxt, sqlqueryresultlist searchlist, bool exactmatch);

};


#endif

