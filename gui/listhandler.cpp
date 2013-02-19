
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

#include "std2qt.h"
#include "listhandler.h"

listhandler::listhandler(mainwindow* pmw)
:mw(pmw)
,m_treeWidgetSearchResults(NULL)
,m_noclick(false)
{
}

listhandler::~listhandler()
{
	disconnect();
}

void listhandler::prepareToExit(void)
{
	disconnect();
	m_noclick = true;
	for(int i=0; i<m_itemlist.size(); i++) delete m_itemlist[i];
	m_itemlist.clear();
	m_treeWidgetSearchResults->clear();
	m_sqlist.resultlist.clear();
}

void listhandler::init(void)
{
	connect(m_treeWidgetSearchResults, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this                     , SLOT(listItemClicked(QTreeWidgetItem *, QTreeWidgetItem *)));
	m_treeWidgetSearchResults->setSelectionMode(QAbstractItemView::SingleSelection);
	m_sqlist.resultlist.clear();
	m_sqlist.result_type = sqlqueryresultlist::sqlresultFULL;
	m_noclick = true;
	updateListHeaders();
	m_noclick = false;
	m_sqlist.result_type = sqlqueryresultlist::sqlresultERROR;
}

void listhandler::populateList(sqlqueryresultlist resultlist)
{
	bool headersChanged = (m_sqlist.result_type != resultlist.result_type);
	m_sqlist = resultlist;
	m_noclick = true;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	clearList();
	if (headersChanged) updateListHeaders();
	updateList();
	resizeColumns();
	QApplication::restoreOverrideCursor();
	m_noclick = false;
	m_treeWidgetSearchResults->setCurrentItem(m_treeWidgetSearchResults->topLevelItem(0));
}

void listhandler::listItemClicked(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	if (current == NULL) return;
	if (m_noclick) return;
	emit openFile(str2qt(m_sqlist.resultlist[current->data(0,Qt::UserRole).toInt()].filepath),
			str2qt(m_sqlist.resultlist[current->data(0,Qt::UserRole).toInt()].linenum));
}

void listhandler::updateList(void)
{
	if (m_sqlist.resultlist.empty()) return;
	for (int i=0; i < m_sqlist.resultlist.size(); i++)
	{
		QStringList strList;
		if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)
			strList += str2qt(m_sqlist.resultlist[i].symname);
		strList += str2qt(m_sqlist.resultlist[i].filename);
		if ((m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)||
			(m_sqlist.result_type == sqlqueryresultlist::sqlresultFILE_LINE))
			strList << str2qt(m_sqlist.resultlist[i].linenum) << str2qt(m_sqlist.resultlist[i].linetext);
		m_itemlist += new QTreeWidgetItem(m_treeWidgetSearchResults, strList);
		m_itemlist[i]->setData(0, Qt::UserRole, QVariant(i));
	}
	m_treeWidgetSearchResults->addTopLevelItems(m_itemlist);
}

void listhandler::updateListHeaders(void)
{
	QStringList headers;
	if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)
		headers += tr("Symbol");
	headers += tr("File");
	if ((m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)||
		(m_sqlist.result_type == sqlqueryresultlist::sqlresultFILE_LINE))
		headers << tr("Line") << tr("Preview");
	m_treeWidgetSearchResults->setHeaderLabels(headers);
	if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)
		{
			m_treeWidgetSearchResults->setColumnCount(4);
		}
	else if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFILE_LINE)
		{
			m_treeWidgetSearchResults->setColumnCount(3);
		}
	else if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFILE_ONLY)
		{
			m_treeWidgetSearchResults->setColumnCount(1);
		}
}

void listhandler::resizeColumns(void)
{
	for(int i=0; i < m_treeWidgetSearchResults->columnCount(); i++)
		{m_treeWidgetSearchResults->resizeColumnToContents(i);}
}

void listhandler::clearList()
{
	bool noclick = m_noclick;
	m_noclick = true;
	for(int i=0; i<m_itemlist.size(); i++) delete m_itemlist[i];
	m_itemlist.clear();
	m_treeWidgetSearchResults->clear();
	m_noclick = noclick;
}

void listhandler::retranslateUi(void)
{
	m_noclick = true;
	if (m_treeWidgetSearchResults->topLevelItemCount() <= 0)
	{
		m_sqlist.result_type = sqlqueryresultlist::sqlresultFULL;
		updateListHeaders();
	}
	else
	{
		int curItemIdx = m_treeWidgetSearchResults->indexOfTopLevelItem(m_treeWidgetSearchResults->currentItem());
		clearList();
		updateListHeaders();
		updateList();
		resizeColumns();
		m_treeWidgetSearchResults->setCurrentItem(m_treeWidgetSearchResults->topLevelItem(curItemIdx));
	}
	m_noclick = false;
}


