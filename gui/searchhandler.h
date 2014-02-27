
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


#ifndef SEARCHHANDLER_H_CQ
#define SEARCHHANDLER_H_CQ

#include <QtGui>
#include "sqlqueryadv.h"

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
QCheckBox *m_checkBoxFilter;
QComboBox *m_comboBoxFilter;
QCompleter *m_completer;
QFutureWatcher<QStringList> m_autocompFutureWatcher;
static bool m_grepExactMatch;
static QRegExp m_grepRegExp;

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
static sqlqueryresultlist doGrep(const QString &fp);
static void collateGrep(sqlqueryresultlist &result,
			const sqlqueryresultlist &intermediate);

public slots:
void OpenDB_ButtonClick(bool checked);
void Search_ButtonClick(bool checked);
void PrevSearch_ButtonClick(bool checked);
void NextSearch_ButtonClick(bool checked);
void ClipSearch_ButtonClick(bool checked);
void Graph_ButtonClick(bool checked);
void Search_EnterKeyPressed();
void searchTextEdited(const QString& searchtxt);
void newSearchText();
void newSearchTextSymbolOnly();
void autoCompleteStateChanged(int state);
void OpenDB_indexChanged(const int& idx);
void QueryType_indexChanged(const int& idx);
void updateListItemRowNum(const int& row);
void resultCurrentListItemSymbolName(const QString symName);
void autoCompleteFinished();

signals:
void searchresults(sqlqueryresultlist resultlist, int selectitem);
void updateStatus(const QString & message, int timeout = 0);
void DBreset();
void sendDBtimestamp(QDateTime dt);
void getResultCurrentListItemSymbolName();

private:
mainwindow *mw;
static sqlqueryadv* sq;
QString m_graphdesc;
int m_typeOfGraph; // 1 = Function Call, 2 = Class Inheritance
QStringListModel m_srchStrLstModel;
QString sqlerrormsg(sqlquery::en_filereadstatus status);
QVector<searchitem> m_searchMemoryList;
QVector<searchitem>::iterator m_iter;
bool m_autocompBusy;
QString m_autocompSrchTerm;

void updateFilterHistory(QString filtertxt);
sqlqueryresultlist perform_grep(QString searchtxt, sqlqueryresultlist searchlist, bool exactmatch);

};


#endif

