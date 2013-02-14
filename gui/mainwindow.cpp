
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


#include "mainwindow.h"
#include "ui_mainWindow.h"
#include "fileviewer.h"
#include "listhandler.h"
#include "searchhandler.h"
#include "swver.h"

#include <QInputDialog>
#include <QMessageBox>

mainwindow::mainwindow(QMainWindow *parent, QApplication *app)
:QMainWindow(parent)
,m_app(app)
,ui(new Ui::MainWindow)
,m_translator(this)
,m_fileviewer(NULL)
,m_listhandler(NULL)
,m_searchhandler(NULL)
,m_currentLanguage("English")
 {
     ui->setupUi(this);
     init();
 }

mainwindow::~mainwindow()
 {
	 disconnect();
     delete m_fileviewer;
     delete m_listhandler;
     delete m_searchhandler;
     delete ui;
 }

void mainwindow::init(void)
{
	m_fileviewer = new fileviewer(this);
	m_listhandler = new listhandler(this);
	m_searchhandler = new searchhandler(this);
	setup_fileviewer();
	setup_listhandler();
	setup_searchhandler();
	connect(m_searchhandler, SIGNAL(searchresults(sqlqueryresultlist)),
			m_listhandler, SLOT(populateList(sqlqueryresultlist)));
	connect(m_listhandler, SIGNAL(openFile(QString, QString)),
			m_fileviewer, SLOT(fileToBeOpened(QString, QString)));
	connect(m_fileviewer, SIGNAL(searchCopiedText()),
			m_searchhandler, SLOT(newSearchText()));
	connect(m_searchhandler, SIGNAL(updateStatus(const QString&, int)),
			ui->statusbar, SLOT(showMessage(const QString&, int)));
	connect(ui->actionExit, SIGNAL(triggered(bool)),
			this, SLOT(ExitTriggered(bool)));
	connect(ui->actionAbout, SIGNAL(triggered(bool)),
			this, SLOT(AboutTriggered(bool)));
	connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)),
			this, SLOT(AboutQtTriggered(bool)));
	connect(ui->actionLanguage, SIGNAL(triggered(bool)),
			this, SLOT(LanguageTriggered(bool)));
	connect(ui->actionOpenDB, SIGNAL(triggered(bool)),
			m_searchhandler, SLOT(OpenDB_ButtonClick(bool)));
	connect(ui->actionOptionsExtEditor, SIGNAL(triggered(bool)),
			m_fileviewer, SLOT(OptionsExtEditor_Triggered(bool)));
	m_app->setQuitOnLastWindowClosed(false);
	connect(m_app, SIGNAL(lastWindowClosed()),
			this, SLOT(prepareToExit()));
}

void mainwindow::setup_fileviewer(void)
{
	m_fileviewer->m_pushButtonPrev = ui->pushButtonPrev;
	m_fileviewer->m_pushButtonNext = ui->pushButtonNext;
	m_fileviewer->m_pushButtonOpenInEditor = ui->pushButtonOpenInEditor;
	m_fileviewer->m_pushButtonPaste = ui->pushButtonPaste;
	m_fileviewer->m_pushButtonGoToLine = ui->pushButtonGoToLine;
	m_fileviewer->m_labelFilePath = ui->labelFilePath;
	m_fileviewer->m_textEditSource = ui->textEditSource;
	m_fileviewer->init();
}

void mainwindow::setup_listhandler(void)
{
	m_listhandler->m_treeWidgetSearchResults = ui->treeWidgetSearchResults;
	m_listhandler->init();
}

void mainwindow::setup_searchhandler(void)
{
	m_searchhandler->m_pushButtonOpenDB = ui->pushButtonOpenDB;
	m_searchhandler->m_comboBoxDB = ui->comboBoxDB;
	m_searchhandler->m_checkBoxAutoComplete = ui->checkBoxAutoComplete;
	m_searchhandler->m_checkBoxExactMatch = ui->checkBoxExactMatch;
	m_searchhandler->m_pushButtonSearch = ui->pushButtonSearch;
	m_searchhandler->m_pushButtonClipSearch = ui->pushButtonClipSearch;
	m_searchhandler->m_comboBoxSearch = ui->comboBoxSearch;
	m_searchhandler->m_comboBoxQueryType = ui->comboBoxQueryType;
	m_searchhandler->init();
}


void mainwindow::LanguageTriggered(bool checked)
{
	QStringList items;
	items << QString("English") << QString("Deutsch") << QString("Francais")
		<< QString("Espanol") << QString("Bahasa Indonesia");

	bool ok;
	int curLangIdx = items.indexOf(m_currentLanguage);
	curLangIdx = curLangIdx != -1 ? curLangIdx : 0;

	QInputDialog qinp(this);
	qinp.setCancelButtonText(tr("Cancel"));
	qinp.setOkButtonText(tr("OK"));
	qinp.setInputMode(QInputDialog::TextInput);
	qinp.setWindowTitle(tr("Language"));
	qinp.setLabelText(tr("Select language:"));
	qinp.setComboBoxEditable(false);
	qinp.setComboBoxItems(items);
	qinp.setTextValue(m_currentLanguage);
	qinp.exec();
	ok = (qinp.result() == QDialog::Accepted);
	QString item = qinp.textValue();
	if (ok && (item.isEmpty() == false))
		{
			m_currentLanguage = item;
			retranslateUi();
		}
}

void mainwindow::retranslateUi(void)
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (m_currentLanguage.compare("English") == 0)
	{
		m_translator.load(":/mainwindow/build/gui/codequery_en");
	}
	else if (m_currentLanguage.compare("Deutsch") == 0)
	{
		m_translator.load(":/mainwindow/build/gui/codequery_de");
	}
	else if (m_currentLanguage.compare("Francais") == 0)
	{
		m_translator.load(":/mainwindow/build/gui/codequery_fr");
	}
	else if (m_currentLanguage.compare("Espanol") == 0)
	{
		m_translator.load(":/mainwindow/build/gui/codequery_es");
	}
	else if (m_currentLanguage.compare("Bahasa Indonesia") == 0)
	{
		m_translator.load(":/mainwindow/build/gui/codequery_id");
	}
	m_app->installTranslator(&m_translator);
	ui->retranslateUi(this);
	m_searchhandler->retranslateUi();
	m_listhandler->retranslateUi();
	QApplication::restoreOverrideCursor();
}

void mainwindow::ExitTriggered(bool checked)
{
	prepareToExit();
}

void mainwindow::AboutQtTriggered(bool checked)
{
	QMessageBox::aboutQt(this);
}

void mainwindow::AboutTriggered(bool checked)
{
	QString txt = CODEQUERY_SW_VERSION;
	txt += "\n";
	txt += CODEQUERY_SW_LICENSE_PARA;
	QMessageBox::about(this, tr("About"), txt);
}

void mainwindow::prepareToExit()
{
	m_listhandler->prepareToExit();
	m_app->quit();
}

