
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef LISTHANDLER_H_CQ
#define LISTHANDLER_H_CQ

#include <QtGlobal>
#include <QtWidgets>

#include "sqlquery.h"

class mainwindow;

class listhandler : public QObject
{
  Q_OBJECT

public:
QTreeWidget *m_treeWidgetSearchResults;
QPushButton *m_pushButtonUp;
QPushButton *m_pushButtonDown;

listhandler(mainwindow* pmw);
~listhandler();
void init(void);
void updateList(void);
void prepareToExit(void);
void updateListHeaders(void);
void resizeColumns(void);
void retranslateUi(void);
void checkUpDown(void);

public slots:
void populateList(sqlqueryresultlist resultlist, int selectitem);
void listItemClicked(QTreeWidgetItem * current, QTreeWidgetItem * previous);
void clearList();
void requestToProvideResultCurrentListItemSymbolName();
void Up_ButtonClick(bool checked);
void Down_ButtonClick(bool checked);

signals:
void openFile(QString file, QString linenum, int fileid);
void listRowNumUpdated(int row);
void sendResultCurrentListItemSymbolName(const QString symName);

private:
mainwindow *mw;
sqlqueryresultlist m_sqlist;
bool m_noclick;

};


#endif

