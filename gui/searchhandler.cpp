
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
#include "searchhandler.h"

searchhandler::searchhandler(mainwindow* pmw)
:mw(pmw)
,sq(new sqlquery)
,m_pushButtonOpenDB(NULL)
,m_comboBoxDB(NULL)
,m_checkBoxAutoComplete(NULL)
,m_checkBoxExactMatch(NULL)
,m_pushButtonSearch(NULL)
,m_comboBoxSearch(NULL)
,m_comboBoxQueryType(NULL)
,m_srchStrLstModel(QStringList())
{
	m_completer = new QCompleter(&m_srchStrLstModel, (QWidget*)mw);
}

searchhandler::~searchhandler()
{
	disconnect();
	delete sq;
	delete m_completer;
}

void searchhandler::OpenDB_ButtonClick(bool checked)
{
	if (!checked) perform_open_db();
}

void searchhandler::Search_ButtonClick(bool checked)
{
	if (!checked) perform_search(m_comboBoxSearch->lineEdit()->text().toAscii().data());
}

void searchhandler::ClipSearch_ButtonClick(bool checked)
{
	if (!checked) newSearchText();
}

void searchhandler::Search_EnterKeyPressed()
{
	perform_search(m_comboBoxSearch->lineEdit()->text().toAscii().data());
}

void searchhandler::searchTextEdited(const QString& searchtxt)
{
	if (m_checkBoxAutoComplete->isChecked())
	{
		m_srchStrLstModel.setStringList(strLst2qt(sq->search_autocomplete(searchtxt.toAscii().data())));
	}
}

void searchhandler::autoCompleteStateChanged(int state)
{
	if (state == Qt::Checked)
	{
		m_srchStrLstModel.setStringList(QStringList(m_comboBoxSearch->lineEdit()->text()));
	}
	else if (state == Qt::Unchecked)
	{
		m_srchStrLstModel.setStringList(QStringList());
	}
}

void searchhandler::newSearchText()
{
	QString txt = (QApplication::clipboard())->text();
	//m_comboBoxSearch->lineEdit()->setText(txt);
	perform_search(txt);
}

void searchhandler::newSearchTextSymbolOnly()
{
	QString txt = (QApplication::clipboard())->text();
	//m_comboBoxSearch->lineEdit()->setText(txt);
	perform_search(txt, sqlquery::sqlquerySYMBOL);
}

void searchhandler::init(void)
{
	m_completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
	m_completer->setCaseSensitivity(Qt::CaseInsensitive);
	m_comboBoxSearch->lineEdit()->setCompleter(m_completer);
	m_comboBoxSearch->setInsertPolicy(QComboBox::NoInsert);
	retranslateUi();
	connect(m_pushButtonOpenDB, SIGNAL(clicked(bool)),
			this, SLOT(OpenDB_ButtonClick(bool)));
	connect(m_pushButtonSearch, SIGNAL(clicked(bool)),
			this, SLOT(Search_ButtonClick(bool)));
	connect(m_pushButtonClipSearch, SIGNAL(clicked(bool)),
			this, SLOT(ClipSearch_ButtonClick(bool)));
	connect(m_comboBoxSearch->lineEdit(), SIGNAL(returnPressed()),
			this, SLOT(Search_EnterKeyPressed()));
	connect(m_comboBoxSearch->lineEdit(), SIGNAL(textEdited(QString)),
			this, SLOT(searchTextEdited(QString)));
	connect(m_comboBoxDB, SIGNAL(currentIndexChanged(int)),
			this, SLOT(OpenDB_indexChanged(int)));
	connect(m_checkBoxAutoComplete, SIGNAL(stateChanged(int)),
			this, SLOT(autoCompleteStateChanged(int)));
}

void searchhandler::retranslateUi(void)
{
	int curidx = 0;
	if (m_comboBoxQueryType->count() > 0) curidx = 
			m_comboBoxQueryType->currentIndex();
	m_comboBoxQueryType->clear();
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Symbol"),
				QVariant(sqlquery::sqlquerySYMBOL));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Function or macro"),
				QVariant(sqlquery::sqlresultFUNC_MACRO));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Class or struct"),
				QVariant(sqlquery::sqlresultCLASS_STRUCT));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Functions calling this function"),
				QVariant(sqlquery::sqlresultCALLINGFUNC));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Functions called by this function"),
				QVariant(sqlquery::sqlresultCALLEDFUNC));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Class which owns this member/ method"),
				QVariant(sqlquery::sqlresultOWNERCLASS));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Members/ methods of this class"),
				QVariant(sqlquery::sqlresultMEMBERS));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Parent of this class"),
				QVariant(sqlquery::sqlresultPARENTCLASS));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Children of this class"),
				QVariant(sqlquery::sqlresultCHILDCLASS));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Files including this file"),
				QVariant(sqlquery::sqlresultINCLUDE));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Full path for file"),
				QVariant(sqlquery::sqlresultFILEPATH));
	m_comboBoxQueryType->setCurrentIndex(curidx);

}

void searchhandler::perform_open_db(void)
{
	QString fileext = tr("CodeQuery DB Files");
	fileext += " (*.db)";
	QString fileName = QFileDialog::getOpenFileName( (QWidget*)mw,
	tr("Open CQ database file"), "", fileext);
	if (fileName.isEmpty() == false)
	{
		m_comboBoxDB->insertItem(0, fileName);
		m_comboBoxDB->setCurrentIndex(0);
		for (int i=1; i < (m_comboBoxDB->count()); i++)
		{
			if (fileName.compare(m_comboBoxDB->itemText(i)) == 0)
			{
				m_comboBoxDB->removeItem(i);
				break;
			}
		}
		if (m_comboBoxDB->count() > 7) m_comboBoxDB->removeItem(m_comboBoxDB->count() - 1);
	}
}

void searchhandler::OpenDB_indexChanged(const int& idx)
{
	if (idx < 0) return;
	sqlquery::en_filereadstatus sqstatus;
	sq->close_dbfile();
	sqstatus = sq->open_dbfile(qt2str(m_comboBoxDB->itemText(idx)));
	if (sqstatus != sqlquery::sqlfileOK)
	{
		QMessageBox msgBox((QWidget*)mw);
		msgBox.setText(sqlerrormsg(sqstatus));
		msgBox.exec();
		m_comboBoxDB->removeItem(idx);
	}
	else
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		m_comboBoxSearch->clear();
		m_comboBoxSearch->lineEdit()->clear();
		emit DBreset();
		QApplication::restoreOverrideCursor();
	}
}

void searchhandler::perform_search(QString searchtxt, sqlquery::en_queryType qrytyp)
{
	if (sq->isDBOpen() == false) return;
	if (searchtxt.isEmpty()) return;
	sqlqueryresultlist sqlresultlist;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	sqlquery::en_queryType querytype = qrytyp;
	if (querytype == sqlquery::sqlresultDEFAULT) querytype = 
		(sqlquery::en_queryType)m_comboBoxQueryType->itemData(m_comboBoxQueryType->currentIndex()).toInt();
	sqlresultlist = sq->search(searchtxt.toAscii().data(),
			querytype, m_checkBoxExactMatch->isChecked());
	QApplication::restoreOverrideCursor();
	if (sqlresultlist.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		QMessageBox msgBox((QWidget*)mw);
		msgBox.setText(str2qt(sqlresultlist.sqlerrmsg));
		msgBox.exec();
	}
	else
	{
		updateSearchHistory(searchtxt);
		emit searchresults(sqlresultlist);
		QString str;
		str = QString("%1").arg(sqlresultlist.resultlist.size());
		str += " ";
		str += tr("results found");
		emit updateStatus(str, 5000);
	}
}

void searchhandler::updateSearchHistory(const QString& searchtxt)
{
	m_comboBoxSearch->insertItem(0, searchtxt); // insert to top
	for(int i=1; i < (m_comboBoxSearch->count()); i++)
	{
		if (m_comboBoxSearch->itemText(i).compare(searchtxt) == 0)
		{
			m_comboBoxSearch->removeItem(i); // remove duplicates
			break;
		}
	}
	// limit to 10 only
	if (m_comboBoxSearch->count() > 10) m_comboBoxSearch->removeItem(m_comboBoxSearch->count() - 1);
	m_comboBoxSearch->setCurrentIndex(0);
}

QString searchhandler::sqlerrormsg(sqlquery::en_filereadstatus status)
{
	QString str;
	switch(status)
	{
		case sqlquery::sqlfileOPENERROR: str = tr("File open error");
		case sqlquery::sqlfileNOTCORRECTDB: str = tr("Wrong file format");
		case sqlquery::sqlfileINCORRECTVER: str = tr("Incorrect CQ database version");
		case sqlquery::sqlfileOK: str = tr("OK");
		case sqlquery::sqlfileUNKNOWNERROR:
		default: str = tr("Unknown Error");
	}
	return str;
}

