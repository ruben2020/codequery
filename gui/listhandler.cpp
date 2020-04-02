
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include "listhandler.h"

listhandler::listhandler(mainwindow* pmw)
:mw(pmw)
,m_treeWidgetSearchResults(NULL)
,m_pushButtonUp(NULL)
,m_pushButtonDown(NULL)
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
	m_treeWidgetSearchResults->clear();
	m_sqlist.resultlist.clear();
}

void listhandler::init(void)
{
	connect(m_treeWidgetSearchResults, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(listItemClicked(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(m_pushButtonUp, SIGNAL(clicked(bool)),
			this, SLOT(Up_ButtonClick(bool)));
	connect(m_pushButtonDown, SIGNAL(clicked(bool)),
			this, SLOT(Down_ButtonClick(bool)));
	m_pushButtonUp->setEnabled(false);
	m_pushButtonDown->setEnabled(false);
	m_treeWidgetSearchResults->setSelectionMode(QAbstractItemView::SingleSelection);
	m_sqlist.resultlist.clear();
	m_sqlist.result_type = sqlqueryresultlist::sqlresultFULL;
	m_noclick = true;
	updateListHeaders();
	m_noclick = false;
	m_sqlist.result_type = sqlqueryresultlist::sqlresultERROR;
}

void listhandler::populateList(sqlqueryresultlist resultlist, int selectitem)
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
	m_treeWidgetSearchResults->setCurrentItem(m_treeWidgetSearchResults->topLevelItem(selectitem));
	emit listRowNumUpdated(selectitem);
}

void listhandler::listItemClicked(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	if (current == NULL) return;
	if (m_noclick) return;
	checkUpDown();
	emit listRowNumUpdated(m_treeWidgetSearchResults->indexOfTopLevelItem(current));
	emit openFile(m_sqlist.resultlist[current->data(0,Qt::UserRole).toLongLong()].filepath,
			m_sqlist.resultlist[current->data(0,Qt::UserRole).toLongLong()].linenum,
			m_sqlist.resultlist[current->data(0,Qt::UserRole).toLongLong()].fileid);
}

void listhandler::requestToProvideResultCurrentListItemSymbolName()
{
	QString symName("");
	if (m_treeWidgetSearchResults->topLevelItemCount() > 0)
	{
		QTreeWidgetItem* current = m_treeWidgetSearchResults->currentItem();
		symName = m_sqlist.resultlist[current->data(0,Qt::UserRole).toLongLong()].symname;
	}
	emit sendResultCurrentListItemSymbolName(symName);
}

void listhandler::updateList(void)
{
	if (m_sqlist.resultlist.empty()) return;
	qlonglong i = 0;
	int col;
	QList<QTreeWidgetItem*> treeitemlist;
	treeitemlist.reserve(m_sqlist.resultlist.size());
	for(std::vector<sqlqueryresult>::iterator it = m_sqlist.resultlist.begin();
		it != m_sqlist.resultlist.end(); it++)
	{
		col = 0;
		treeitemlist += new QTreeWidgetItem(m_treeWidgetSearchResults);
		if (m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)
			treeitemlist.last()->setText(col++, it->symname);
		treeitemlist.last()->setText(col++, it->filename);
		if ((m_sqlist.result_type == sqlqueryresultlist::sqlresultFULL)||
			(m_sqlist.result_type == sqlqueryresultlist::sqlresultFILE_LINE))
		{
			treeitemlist.last()->setText(col++, it->linenum);
			treeitemlist.last()->setText(col++, it->linetext);
		}
		treeitemlist.last()->setData(0, Qt::UserRole, QVariant(i++));
	}
	m_treeWidgetSearchResults->addTopLevelItems(treeitemlist);
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
	int n = m_treeWidgetSearchResults->columnCount();
	if (n > 1) n--;
	for(int i=0; i < n; i++)
		{m_treeWidgetSearchResults->resizeColumnToContents(i);}
}

void listhandler::clearList()
{
	bool noclick = m_noclick;
	m_pushButtonUp->setEnabled(false);
	m_pushButtonDown->setEnabled(false);
	m_noclick = true;
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
		m_noclick = false;
	}
	else
	{
		int curItemIdx = m_treeWidgetSearchResults->indexOfTopLevelItem(m_treeWidgetSearchResults->currentItem());
		clearList();
		updateListHeaders();
		updateList();
		resizeColumns();
		m_treeWidgetSearchResults->setCurrentItem(m_treeWidgetSearchResults->topLevelItem(curItemIdx));
		m_noclick = false;
		listItemClicked(m_treeWidgetSearchResults->topLevelItem(curItemIdx), NULL);
	}
}

void listhandler::Up_ButtonClick(bool checked)
{
	QTreeWidgetItem *item = NULL;
	tStr curFilepath;
	tStr itemFilepath;
	if ((!checked)&&(m_treeWidgetSearchResults->topLevelItemCount() > 0))
	{
		item = m_treeWidgetSearchResults->currentItem();
		if (item != NULL)
			curFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
		while (item != NULL)
		{
			item = m_treeWidgetSearchResults->itemAbove(item);
			if (item == NULL) break;
			itemFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
			if (strrevcmp(curFilepath, itemFilepath))
			{
				m_treeWidgetSearchResults->setCurrentItem(item);
				break;
			}
		}
	}
}

void listhandler::Down_ButtonClick(bool checked)
{
	QTreeWidgetItem *item = NULL;
	tStr curFilepath;
	tStr itemFilepath;
	if ((!checked)&&(m_treeWidgetSearchResults->topLevelItemCount() > 0))
	{
		item = m_treeWidgetSearchResults->currentItem();
		if (item != NULL)
			curFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
		while (item != NULL)
		{
			item = m_treeWidgetSearchResults->itemBelow(item);
			if (item == NULL) break;
			itemFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
			if (strrevcmp(curFilepath, itemFilepath))
			{
				m_treeWidgetSearchResults->setCurrentItem(item);
				break;
			}
		}
	}
}

void listhandler::checkUpDown(void)
{
	QTreeWidgetItem *item = NULL;
	QTreeWidgetItem *curItem = NULL;
	tStr curFilepath, itemFilepath;
	bool upArrow=false, downArrow=false;
	if (m_treeWidgetSearchResults->topLevelItemCount() <= 0) return;
	item = m_treeWidgetSearchResults->currentItem();
	curItem = item;
	if (item != NULL)
		curFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;

	while (item != NULL)
	{
		item = m_treeWidgetSearchResults->itemAbove(item);
		if (item == NULL) break;
		itemFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
		if (strrevcmp(curFilepath, itemFilepath))
		{
			upArrow = true;
			break;
		}

		// for now, assume items with same file path are grouped together
		// This is to speed up the search
		else break;
	}

	item = curItem;
	while (item != NULL)
	{
		item = m_treeWidgetSearchResults->itemBelow(item);
		if (item == NULL) break;
		itemFilepath = m_sqlist.resultlist[item->data(0,Qt::UserRole).toLongLong()].filepath;
		if (strrevcmp(curFilepath, itemFilepath))
		{
			downArrow = true;
			break;
		}

		// for now, assume items with same file path are grouped together
		// This is to speed up the search
		else break;
	}

	m_pushButtonUp->setEnabled(upArrow);
	m_pushButtonDown->setEnabled(downArrow);
}

