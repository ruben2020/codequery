
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



#ifndef LISTHANDLER_H_CQ
#define LISTHANDLER_H_CQ

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
void openFile(QString file, QString linenum);
void listRowNumUpdated(int row);
void sendResultCurrentListItemSymbolName(const QString symName);

private:
mainwindow *mw;
sqlqueryresultlist m_sqlist;
QList<QTreeWidgetItem*> m_itemlist;
bool m_noclick;

};


#endif

