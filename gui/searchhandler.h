
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


#ifndef _SEARCHHANDLER_H
#define _SEARCHHANDLER_H

#include <QtGui>
#include "sqlquery.h"

class mainwindow;

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
QComboBox *m_comboBoxSearch;
QComboBox *m_comboBoxQueryType;
QCompleter *m_completer;

searchhandler(mainwindow* pmw);
~searchhandler();
void init(void);
void perform_open_db(void);
void perform_search(QString searchtxt, sqlquery::en_queryType qrytyp = sqlquery::sqlresultDEFAULT);
void updateSearchHistory(const QString& searchtxt);
void retranslateUi(void);

public slots:
void OpenDB_ButtonClick(bool checked);
void Search_ButtonClick(bool checked);
void ClipSearch_ButtonClick(bool checked);
void Search_EnterKeyPressed();
void searchTextEdited(const QString& searchtxt);
void newSearchText();
void newSearchTextSymbolOnly();
void autoCompleteStateChanged(int state);
void OpenDB_indexChanged(const int& idx);

signals:
void searchresults(sqlqueryresultlist resultlist);
void updateStatus(const QString & message, int timeout = 0);
void DBreset();

private:
mainwindow *mw;
sqlquery* sq;
QStringListModel m_srchStrLstModel;
QString sqlerrormsg(sqlquery::en_filereadstatus status);

};


#endif
