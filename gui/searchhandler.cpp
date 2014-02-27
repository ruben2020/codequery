	
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


#include <QVector>
#include "std2qt.h"
#include "graphdialog.h"
#include "searchhandler.h"

sqlqueryadv* searchhandler::sq = NULL;
bool searchhandler::m_grepExactMatch = false;
QRegExp* searchhandler::m_grepRegExp = NULL;

searchitem::searchitem()
:exactmatch(false)
,qtype(sqlquery::sqlquerySYMBOL)
,rownum(1)
{
}

searchitem::searchitem(const searchitem& otheritem)
{
	searchterm = otheritem.searchterm;
	filterterm = otheritem.filterterm;
	exactmatch = otheritem.exactmatch;
	qtype = otheritem.qtype;
	rownum = otheritem.rownum;
}

searchitem& searchitem::operator=(const searchitem& otheritem)
{
	if (&otheritem != this)
	{
		searchterm = otheritem.searchterm;
		filterterm = otheritem.filterterm;
		exactmatch = otheritem.exactmatch;
		qtype = otheritem.qtype;
		rownum = otheritem.rownum;
	}
	return *this;
}

// return value: 0=same, 1=completely different, 2=only linenum changed
int searchitem::compare(const searchitem& otheritem)
{
	if ((searchterm.compare(otheritem.searchterm, Qt::CaseSensitive) != 0) ||
		(exactmatch != otheritem.exactmatch) ||
		(qtype != otheritem.qtype) ||
		(filterterm.compare(otheritem.filterterm, Qt::CaseSensitive) != 0)) return 1;
	if (rownum != otheritem.rownum) return 2;
	return 0;
}


searchhandler::searchhandler(mainwindow* pmw)
:mw(pmw)
,m_pushButtonOpenDB(NULL)
,m_comboBoxDB(NULL)
,m_checkBoxAutoComplete(NULL)
,m_checkBoxExactMatch(NULL)
,m_pushButtonSearch(NULL)
,m_comboBoxSearch(NULL)
,m_comboBoxQueryType(NULL)
,m_checkBoxFilter(NULL)
,m_comboBoxFilter(NULL)
,m_srchStrLstModel(QStringList())
,m_typeOfGraph(1)
,m_autocompBusy(false)
{
	sq = new sqlqueryadv;
	m_completer = new QCompleter(&m_srchStrLstModel, (QWidget*)mw);
	m_grepRegExp = new QRegExp();
	m_iter = m_searchMemoryList.begin();
}

searchhandler::~searchhandler()
{
	disconnect();
	delete sq;
	delete m_completer;
	delete m_grepRegExp;
}

void searchhandler::OpenDB_ButtonClick(bool checked)
{
	if (!checked) perform_open_db();
}

void searchhandler::Search_ButtonClick(bool checked)
{
	if (!checked) perform_search(m_comboBoxSearch->lineEdit()->text().trimmed().toAscii().data(),
					m_checkBoxExactMatch->isChecked());
}

void searchhandler::ClipSearch_ButtonClick(bool checked)
{
	if (!checked) newSearchText();
}

void searchhandler::Graph_ButtonClick(bool checked)
{
	if (!checked)
	{
		emit getResultCurrentListItemSymbolName();
	}
}

void searchhandler::PrevSearch_ButtonClick(bool checked)
{
	if (!checked) goBackInSearchMemory();
}

void searchhandler::NextSearch_ButtonClick(bool checked)
{
	if (!checked) goForwardInSearchMemory();
}

void searchhandler::Search_EnterKeyPressed()
{
	perform_search(m_comboBoxSearch->lineEdit()->text().trimmed().toAscii().data(),
			m_checkBoxExactMatch->isChecked());
}

void searchhandler::searchTextEdited(const QString& searchtxt)
{
	if (m_checkBoxAutoComplete->isChecked())
	{
		if (m_autocompBusy)
		{
			m_autocompSrchTerm = searchtxt;
		}
		else
		{
			m_autocompBusy = true;
			m_autocompSrchTerm.clear();
			m_autocompFutureWatcher.setFuture(
				QtConcurrent::run(search_autocomplete_qt, searchtxt));
		}
	}
	m_autocompBusy = (!m_autocompFutureWatcher.isFinished());
}

void searchhandler::autoCompleteFinished()
{
	if (!m_autocompBusy) return;
	m_srchStrLstModel.setStringList(m_autocompFutureWatcher.result());
	if (m_autocompSrchTerm.isEmpty())
	{
		m_autocompBusy = false;
	}
	else
	{
		m_autocompBusy = true;
		m_autocompFutureWatcher.setFuture(
			QtConcurrent::run(search_autocomplete_qt, m_autocompSrchTerm));
		m_autocompSrchTerm.clear();
	}
}

QStringList searchhandler::search_autocomplete_qt(QString searchtxt)
{
	return strLst2qt(sq->search_autocomplete(searchtxt.toAscii().data()));
}

void searchhandler::autoCompleteStateChanged(int state)
{
	if (state == Qt::Checked)
	{
		m_srchStrLstModel.setStringList(QStringList(m_comboBoxSearch->lineEdit()->text().trimmed()));
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
	perform_search(txt,m_checkBoxExactMatch->isChecked());
}

void searchhandler::newSearchTextSymbolOnly()
{
	QString txt = (QApplication::clipboard())->text();
	//m_comboBoxSearch->lineEdit()->setText(txt);
	perform_search(txt, m_checkBoxExactMatch->isChecked(), sqlquery::sqlquerySYMBOL);
}

void searchhandler::init(void)
{
	m_completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
	m_completer->setCaseSensitivity(Qt::CaseInsensitive);
	m_comboBoxSearch->lineEdit()->setCompleter(m_completer);
	m_comboBoxSearch->setInsertPolicy(QComboBox::NoInsert);
	retranslateUi();
	m_pushButtonSearchPrev->setEnabled(false);
	m_pushButtonSearchNext->setEnabled(false);
	connect(m_pushButtonOpenDB, SIGNAL(clicked(bool)),
			this, SLOT(OpenDB_ButtonClick(bool)));
	connect(m_pushButtonSearch, SIGNAL(clicked(bool)),
			this, SLOT(Search_ButtonClick(bool)));
	connect(m_pushButtonClipSearch, SIGNAL(clicked(bool)),
			this, SLOT(ClipSearch_ButtonClick(bool)));
	connect(m_pushButtonGraph, SIGNAL(clicked(bool)),
			this, SLOT(Graph_ButtonClick(bool)));
	connect(m_comboBoxSearch->lineEdit(), SIGNAL(returnPressed()),
			this, SLOT(Search_EnterKeyPressed()));
	connect(m_comboBoxSearch->lineEdit(), SIGNAL(textEdited(QString)),
			this, SLOT(searchTextEdited(QString)));
	connect(m_comboBoxDB, SIGNAL(currentIndexChanged(int)),
			this, SLOT(OpenDB_indexChanged(int)));
	connect(m_comboBoxQueryType, SIGNAL(currentIndexChanged(int)),
			this, SLOT(QueryType_indexChanged(int)));
	connect(m_checkBoxAutoComplete, SIGNAL(stateChanged(int)),
			this, SLOT(autoCompleteStateChanged(int)));
	connect(m_pushButtonSearchPrev, SIGNAL(clicked(bool)),
			this, SLOT(PrevSearch_ButtonClick(bool)));
	connect(m_pushButtonSearchNext, SIGNAL(clicked(bool)),
			this, SLOT(NextSearch_ButtonClick(bool)));
	connect(&m_autocompFutureWatcher, SIGNAL(finished()),
			this, SLOT(autoCompleteFinished()));
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
				tr("Function or macro definition (Graph available)"),
				QVariant(sqlquery::sqlresultFUNC_MACRO));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Calls of this function or macro"),
				QVariant(sqlquery::sqlresultCALLSOFFUNC));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Functions calling this function"),
				QVariant(sqlquery::sqlresultCALLINGFUNC));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Functions called by this function"),
				QVariant(sqlquery::sqlresultCALLEDFUNC));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Class or struct (Graph available)"),
				QVariant(sqlquery::sqlresultCLASS_STRUCT));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Class which owns this member or method"),
				QVariant(sqlquery::sqlresultOWNERCLASS));
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Members or methods of this class"),
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
	m_comboBoxQueryType->addItem(QIcon(),
				tr("Functions or macros inside this file"),
				QVariant(sqlquery::sqlresultFUNCSINFILE));
	m_comboBoxQueryType->addItem(QIcon(),
				"Grep",
				QVariant(sqlquery::sqlresultGREP));
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
			if (fileName.compare(m_comboBoxDB->itemText(i), Qt::CaseSensitive) == 0)
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
	if (m_autocompBusy)
	{
		m_autocompBusy = false;
		m_autocompFutureWatcher.waitForFinished();
	}
	sq->close_dbfile();
	QFileInfo dbfile(m_comboBoxDB->itemText(idx));
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
		m_searchMemoryList.clear();
		m_iter = m_searchMemoryList.begin();
		m_pushButtonSearchPrev->setEnabled(false);
		m_pushButtonSearchNext->setEnabled(false);
		emit sendDBtimestamp(dbfile.lastModified());
		emit DBreset();
		QApplication::restoreOverrideCursor();
	}
}

void searchhandler::QueryType_indexChanged(const int& idx)
{
	m_pushButtonGraph->setEnabled(false);
	switch(m_comboBoxQueryType->itemData(idx).toInt())
	{
		case sqlquery::sqlresultFUNC_MACRO: // function or macro
			m_graphdesc = tr("Function Call Graph");
			m_typeOfGraph = 1;
			break;
		case sqlquery::sqlresultCLASS_STRUCT: // class or struct
			m_graphdesc = tr("Class Inheritance Graph");
			m_typeOfGraph = 2;
			break;
	}
}

void searchhandler::perform_search(QString searchtxt,
			bool exactmatch,
			sqlquery::en_queryType qrytyp,
			QString filtertxt,
			int selectitem,
			bool updSearchMemory)
{
	if (sq->isDBOpen() == false) return;
	if (searchtxt.isEmpty()) return;
	sqlqueryresultlist sqlresultlist;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (m_autocompBusy)
	{
		m_autocompBusy = false;
		m_autocompFutureWatcher.waitForFinished();
	}
	sqlquery::en_queryType querytype = qrytyp;
	if (querytype == sqlquery::sqlresultDEFAULT) querytype = 
		(sqlquery::en_queryType)m_comboBoxQueryType->itemData(m_comboBoxQueryType->currentIndex()).toInt();
	if ((filtertxt.isEmpty()) && (m_checkBoxFilter->isChecked()))
	{
		filtertxt = m_comboBoxFilter->lineEdit()->text().trimmed();
		if (updSearchMemory) updateFilterHistory(filtertxt);
	}
	if (querytype == sqlquery::sqlresultGREP)
	{
		if (filtertxt.isEmpty()) filtertxt = "*";
		sqlresultlist = sq->search(filtertxt.toAscii().data(),
				sqlquery::sqlresultFILEPATH, false);
	}
	else
	{
		sqlresultlist = sq->search(searchtxt.toAscii().data(),
				querytype, exactmatch,
				filtertxt.toAscii().data());
	}
	QApplication::restoreOverrideCursor();
	if (sqlresultlist.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		QMessageBox msgBox((QWidget*)mw);
		msgBox.setText(str2qt(sqlresultlist.sqlerrmsg));
		msgBox.exec();
	}
	else
	{
		m_pushButtonGraph->setEnabled((querytype == sqlquery::sqlresultFUNC_MACRO)||
			(querytype == sqlquery::sqlresultCLASS_STRUCT));
		updateSearchHistory(searchtxt);
		if (updSearchMemory) addToSearchMemory(searchtxt, filtertxt);
		if (querytype == sqlquery::sqlresultGREP)
		{
			sqlresultlist = perform_grep(searchtxt, sqlresultlist, exactmatch);
		}
		emit searchresults(sqlresultlist, selectitem);
		QString str;
		str = QString("%1").arg(sqlresultlist.resultlist.size());
		str += " ";
		str += tr("results found");
		emit updateStatus(str, 5000);
	}
}

sqlqueryresultlist searchhandler::perform_grep(QString searchtxt, sqlqueryresultlist searchlist, bool exactmatch)
{
	QVector<QString> strvec;
	sqlqueryresultlist resultlist;
	QFutureWatcher<sqlqueryresultlist> futureWatcher;
	QProgressDialog dialog;
	unsigned int n = searchlist.resultlist.size();
	if (n == 0) return resultlist;
	strvec.resize(n);
	for (unsigned int i=0; i < n; i++)
	{
		strvec.replace(i, str2qt(searchlist.resultlist[i].filepath));
	}
	dialog.setAutoReset(false);
	dialog.setLabelText(QString("Grep ").append(QString(tr("in progress"))).append(QString(" ...")));
	dialog.setCancelButtonText(tr("Cancel"));
	QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
	QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
	m_grepExactMatch = exactmatch;
	(*m_grepRegExp) = QRegExp(searchtxt.toAscii().data(), Qt::CaseInsensitive);
	m_grepRegExp->setPatternSyntax(QRegExp::RegExp2);
	futureWatcher.setFuture(QtConcurrent::mappedReduced(strvec, doGrep,
				collateGrep, QtConcurrent::SequentialReduce));
	dialog.exec();
	futureWatcher.waitForFinished();
	if (futureWatcher.isCanceled() == false)
		resultlist = futureWatcher.result();
	return resultlist;
}

sqlqueryresultlist searchhandler::doGrep(const QString &fp)
{
	sqlqueryresultlist reslist;
	sqlqueryresult res;
	QString str, fp2;
	tStr fpstr, fn;
	int pos, linenumber=0;
	char numtext[10];
	QRegExp rx1(*m_grepRegExp);
	reslist.result_type = sqlqueryresultlist::sqlresultFILE_LINE;
	fp2 = fp;
	fp2.replace(QString("$HOME"), QDir::homePath());
#ifdef _WIN32
	fp2.replace("/", "\\");
#endif
	fpstr = qt2str(fp2);
	fn = extract_filename(fpstr.c_str());
	QFile file(fp2);
	QTextStream in(&file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return reslist;
	}
	while (!in.atEnd())
	{
		linenumber++;
		str = in.readLine();
		pos = rx1.indexIn(str);
		if (pos != -1)
		{
			res.filepath = fpstr;
			res.filename = fn;
			sprintf(numtext, "%d", linenumber);
			res.linenum = numtext;
			res.linetext = qt2str(str.trimmed().left(80));
			reslist.resultlist.push_back(res);
		}
	}
	return reslist;
}

void searchhandler::collateGrep(sqlqueryresultlist &result,
			const sqlqueryresultlist &intermediate)
{
	unsigned int i, n;
	n = intermediate.resultlist.size();
	result.result_type = sqlqueryresultlist::sqlresultFILE_LINE;
	result.resultlist.reserve(n);
	for(i=0; i<n; i++)
	{
		result.resultlist.push_back(intermediate.resultlist[i]);
	}
}

void searchhandler::updateFilterHistory(QString filtertxt)
{
	if (filtertxt.isEmpty()) return;
	m_comboBoxFilter->insertItem(0, filtertxt);
	m_comboBoxFilter->setCurrentIndex(0);
	for(int i=1; i < m_comboBoxFilter->count(); i++)
	{
		if (m_comboBoxFilter->itemText(i).compare(filtertxt) == 0)
		{
			m_comboBoxFilter->removeItem(i);
			break;
		}
	}
	if (m_comboBoxFilter->count() > 7) // limit to 7
	{
		m_comboBoxFilter->removeItem(m_comboBoxFilter->count() - 1);
	}
}

void searchhandler::resultCurrentListItemSymbolName(const QString symName)
{
	if (symName.isEmpty())
	{
		QMessageBox msgBox((QWidget*)mw);
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(tr("You have to first select an item from the list before pushing the Graph button."));
		msgBox.exec();
		return;
	}	

	QString grpxml, grpdot;
	bool res;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (m_autocompBusy)
	{
		m_autocompBusy = false;
		m_autocompFutureWatcher.waitForFinished();
	}
	if (m_typeOfGraph == 1)
	res = sq->search_funcgraph(symName,
			true,
			grpxml, grpdot);
	else if (m_typeOfGraph == 2)
	res = sq->search_classinheritgraph(symName,
			true,
			grpxml, grpdot);
	else {QApplication::restoreOverrideCursor(); return;}
	QApplication::restoreOverrideCursor();
	QMessageBox msgBox((QWidget*)mw);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStandardButtons(QMessageBox::Ok);
	if (!res)
	{
		msgBox.setText("SQL Query error");
		msgBox.exec();
		return;
	}
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	cqDialogGraph cqdg((QWidget*)mw);
	cqdg.setModal(true);
	cqdg.setupGraphFromXML(grpxml, grpdot, m_graphdesc);
	QApplication::restoreOverrideCursor();
	cqdg.exec();
}

void searchhandler::addToSearchMemory(const QString& searchtxt, const QString& filtertxt)
{
	searchitem item;
	item.searchterm = searchtxt;
	item.filterterm = filtertxt;
	item.exactmatch = m_checkBoxExactMatch->isChecked();
	item.qtype = (sqlquery::en_queryType)
		m_comboBoxQueryType->itemData(m_comboBoxQueryType->currentIndex()).toInt();
	m_searchMemoryList.push_back(item);
	if (m_searchMemoryList.size() > 20) m_searchMemoryList.erase(m_searchMemoryList.begin());
	m_iter = m_searchMemoryList.end() - 1;
	m_pushButtonSearchPrev->setEnabled(m_searchMemoryList.isEmpty() == false);
	m_pushButtonSearchNext->setEnabled(false);
}

void searchhandler::goForwardInSearchMemory(void)
{
	if (m_searchMemoryList.size() <= 1) return;
	if (m_iter == m_searchMemoryList.end() - 1) return;
	m_iter++;
	m_pushButtonSearchPrev->setEnabled(m_searchMemoryList.isEmpty() == false);
	m_pushButtonSearchNext->setEnabled(m_iter != m_searchMemoryList.end() - 1);
	restoreSearchMemoryItem();
}

void searchhandler::goBackInSearchMemory(void)
{
	if (m_searchMemoryList.size() <= 1) return;
	if (m_iter == m_searchMemoryList.begin()) return;
	m_iter--;
	m_pushButtonSearchPrev->setEnabled(m_iter != m_searchMemoryList.begin());
	m_pushButtonSearchNext->setEnabled(m_searchMemoryList.isEmpty() == false);
	restoreSearchMemoryItem();
}

void searchhandler::restoreSearchMemoryItem(void)
{
	perform_search(m_iter->searchterm,
			m_iter->exactmatch,
			m_iter->qtype,
			m_iter->filterterm,
			m_iter->rownum,
			false);
}

void searchhandler::updateListItemRowNum(const int& row)
{
	if (m_searchMemoryList.isEmpty() == false) m_iter->rownum = row;
}

void searchhandler::updateSearchHistory(const QString& searchtxt)
{
	m_comboBoxSearch->insertItem(0, searchtxt); // insert to top
	int n = (m_comboBoxSearch->count());
	for(int i=1; i < n; i++)
	{
		if (m_comboBoxSearch->itemText(i).compare(searchtxt, Qt::CaseSensitive) == 0)
		{
			m_comboBoxSearch->removeItem(i); // remove duplicates
			break;
		}
	}
	// limit to 15 only
	if (m_comboBoxSearch->count() > 15) m_comboBoxSearch->removeItem(m_comboBoxSearch->count() - 1);
	m_comboBoxSearch->setCurrentIndex(0);
}

QString searchhandler::sqlerrormsg(sqlquery::en_filereadstatus status)
{
	QString str;
	switch(status)
	{
		case sqlquery::sqlfileOPENERROR: str = tr("File open error"); break;
		case sqlquery::sqlfileNOTCORRECTDB: str = tr("Wrong file format"); break;
		case sqlquery::sqlfileINCORRECTVER: str = tr("Incorrect CQ database version"); break;
		case sqlquery::sqlfileOK: str = tr("OK"); break;
		case sqlquery::sqlfileUNKNOWNERROR:
		default: str = tr("Unknown Error"); break;
	}
	return str;
}

