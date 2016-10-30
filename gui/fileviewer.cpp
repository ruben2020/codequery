
/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#include <stdlib.h>
#include <string.h>
#include <QProcess>
#include <QInputDialog>
#include <QFontDatabase>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerjavascript.h>

#include "fileviewer.h"
#include "mainwindow.h"
#include "fileviewsettingsdialog.h"
#include "themes.h"
#include "std2qt.h"

#ifdef USE_QT5
#define QT45_TOASCII(x) toLatin1(x)
#else
#define QT45_TOASCII(x) toAscii(x)
#endif

#ifdef _WIN32
#define EXT_EDITOR_DEFAULT_PATH "notepad %f"
#else
#define EXT_EDITOR_DEFAULT_PATH "gedit %f +%n"
#endif

filedata::filedata()
{
	linenum = "1";
	fileid = -99;
}
filedata::filedata(const QString& fn, const QString& ln, const int& fi)
{
	linenum = ln;
	filename = fn;
	fileid = fi;
}

bool filedata::compare(const filedata& fd)
{
	bool cmp;
	if ((fileid < 0)||(fd.fileid < 0))
	{
		cmp = ((linenum.compare(fd.linenum) == 0)&&
			(filename.compare(fd.filename) == 0));
	}
	else
	{
		cmp = ((linenum.compare(fd.linenum) == 0)&&
			(fileid == fd.fileid));
	}
	return cmp;
}

bool filedata::compareFilePathOnly(const filedata& fd)
{
	bool cmp;
	if ((fileid < 0)||(fd.fileid < 0))
	{
		cmp = (filename.compare(fd.filename) == 0);
	}
	else
	{
		cmp = (fileid == fd.fileid);
	}
	return cmp;
}

bool filedata::compareFileNameOnly(const filedata& fd)
{
	bool cmp;
	if ((fileid < 0)||(fd.fileid < 0))
	{
		cmp =(strcmp(
		extract_filename(filename.QT45_TOASCII().data()),
		extract_filename(fd.filename.QT45_TOASCII().data())) == 0);
	}
	else
	{
		cmp = (fileid == fd.fileid);
	}
	return cmp;
}

filedata::filedata(const filedata& fd)
{
	linenum = fd.linenum;
	filename = fd.filename;
	fileid = fd.fileid;
}

filedata& filedata::operator=(const filedata& fd)
{
	if (&fd != this)
	{
		linenum = fd.linenum;
		filename = fd.filename;
		fileid = fd.fileid;
	}
	return *this;
}

fileviewer::fileviewer(mainwindow* pmw)
:mw(pmw)
,m_pushButtonPrev(NULL)
,m_pushButtonNext(NULL)
,m_pushButtonOpenInEditor(NULL)
,m_pushButtonPaste(NULL)
,m_pushButtonGoToLine(NULL)
,m_labelFilePath(NULL)
,m_textEditSource(NULL)
,m_textEditSourceFont("Courier New", 12)
,m_externalEditorPath(EXT_EDITOR_DEFAULT_PATH)
,m_timestampMismatchWarned(false)
,m_lexer(NULL)
,m_fontsize(0)
,m_currentlang(enHighlightCPP)
,m_currentline(1)
,m_annotline(1)
,m_pushButtonTextShrink(NULL)
,m_pushButtonTextEnlarge(NULL)
,m_checkBoxSymbolOnly(NULL)
,m_listWidgetFunc(NULL)
,m_comboBoxFuncListSort(NULL)
,m_fontwidthtemp(1)
,m_markerhandle(1)
,m_markerhandle2(2)
{
	m_iter = m_fileDataList.begin();
	m_textEditSourceFont.setStyleHint(QFont::TypeWriter);	
}

fileviewer::~fileviewer()
{
	disconnect();
	if (m_lexer != NULL) delete m_lexer;
}

void fileviewer::createFontList(void)
{
	QFontDatabase fontdb;
	QStringList fontlst = fontdb.families(QFontDatabase::Latin);
	QStringList fixedpitch;
	QStringList::iterator it;
	for(it = fontlst.begin(); it != fontlst.end(); it++)
	{
		if (fontdb.isFixedPitch(*it))
		{
			fixedpitch << (*it);
		}
	}
	fixedpitch.sort();
	m_fontlist = fixedpitch;
}

void fileviewer::init(void)
{
	m_pushButtonPaste->setEnabled(false);
	m_pushButtonPrev->setEnabled(false);
	m_pushButtonNext->setEnabled(false);
	m_pushButtonTextShrink->setEnabled(false);
	m_pushButtonTextEnlarge->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);
	m_pushButtonOpenInEditor->setEnabled(false);
	m_labelFilePath->clear();
	m_textEditSource->clear();
	m_textEditSource->setWrapMode(QsciScintilla::WrapNone);
	m_textEditSource->setReadOnly(true);
	m_markerhandle = m_textEditSource->markerDefine(QsciScintilla::Background);
	m_markerhandle2 = m_textEditSource->markerDefine(QsciScintilla::RightArrow);
	m_textEditSource->setMarginType(0, QsciScintilla::NumberMargin);
	m_textEditSource->setMarginType(1, QsciScintilla::SymbolMargin);
	m_textEditSource->setBraceMatching(QsciScintilla::SloppyBraceMatch);
	m_textEditSource->setAnnotationDisplay(QsciScintilla::AnnotationBoxed);
	setLexer(enHighlightCPP);
	createFontList(); 
	connect(m_textEditSource, SIGNAL(copyAvailable(bool)),
			this, SLOT(AbleToCopy(bool)));
	connect(m_pushButtonGoToLine, SIGNAL(clicked(bool)),
			this, SLOT(GoToLine_ButtonClick(bool)));
	connect(m_pushButtonPaste, SIGNAL(clicked(bool)),
			this, SLOT(Paste_ButtonClick(bool)));
	connect(m_pushButtonPrev, SIGNAL(clicked(bool)),
			this, SLOT(Prev_ButtonClick(bool)));
	connect(m_pushButtonNext, SIGNAL(clicked(bool)),
			this, SLOT(Next_ButtonClick(bool)));
	connect(m_pushButtonOpenInEditor, SIGNAL(clicked(bool)),
			this, SLOT(OpenInEditor_ButtonClick(bool)));
	connect(m_pushButtonTextShrink, SIGNAL(clicked(bool)),
			this, SLOT(TextShrink_ButtonClick(bool)));
	connect(m_pushButtonTextEnlarge, SIGNAL(clicked(bool)),
			this, SLOT(TextEnlarge_ButtonClick(bool)));
	connect(m_listWidgetFunc, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
			this, SLOT(funcItemSelected(QListWidgetItem *, QListWidgetItem *)));
	connect(m_comboBoxFuncListSort, SIGNAL(currentIndexChanged(int)),
			this, SLOT(FuncListSort_indexChanged(int)));
	m_fileDataList.clear();
	setLexer();
}

void fileviewer::clearList()
{
	//printf("Fileview clearlist\n");
	m_pushButtonPaste->setEnabled(false);
	m_pushButtonPrev->setEnabled(false);
	m_pushButtonNext->setEnabled(false);
	m_pushButtonTextShrink->setEnabled(false);
	m_pushButtonTextEnlarge->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);
	m_pushButtonOpenInEditor->setEnabled(false);
	m_labelFilePath->clear();
	m_textEditSource->clear();
	m_fileDataList.clear();
	m_iter = m_fileDataList.begin();
	m_timestampMismatchWarned = false;
	m_listWidgetFunc->clear();
}

void fileviewer::recvDBtimestamp(QDateTime dt)
{
	m_DBtimestamp = dt;
}

void fileviewer::fileToBeOpened(QString filename, QString linenum, int fileid)
{
	filename.replace(QString("$HOME"), QDir::homePath());
#ifdef _WIN32
	filename.replace("/", "\\");
#endif
	if (!(QFile::exists(filename)))
	{
		m_labelFilePath->setText(tr("File not found"));
		handleFileCannotBeOpenedCase();
		return;
	}
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_labelFilePath->setText(tr("File could not be opened"));
		handleFileCannotBeOpenedCase();
		return;
	}
	file.close();

	QFileInfo fi(filename);
	if ((m_DBtimestamp < fi.lastModified())&&(m_timestampMismatchWarned == false))
	{
		
		m_timestampMismatchWarned = true;
		QMessageBox msgBox((QWidget*)mw);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(tr("The source file to be viewed is newer than the CodeQuery database file. You are recommended to manually regenerate the CodeQuery database file."));
		msgBox.exec();
	}
	filedata fd(filename, linenum, fileid);
	if (m_fileDataList.isEmpty())
	{
		//printf("Fileviewer: empty list\n");
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(false);
		m_pushButtonNext->setEnabled(false);
		updateTextEdit();
		return;
	}
	else if (m_iter == m_fileDataList.end())
	{
		// previous file not found
		//printf("Fileviewer: previous file not found %d, %d\n", m_fileDataList.end(), m_fileDataList.end() - 1);
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(m_iter != m_fileDataList.begin());
		m_pushButtonNext->setEnabled(false);
		updateTextEdit();		
	}
	else if (m_iter->compare(fd))
	{
		// the same filename and line number
		//printf("Fileviewer: same filename and line number\n");
		updateFilePathLabel();
		return;
	}
	else if (m_iter->compareFilePathOnly(fd))
	{
		// same file, different line number
		//printf("Fileviewer: same file, different line number\n");
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(true);
		m_pushButtonNext->setEnabled(false);
		highlightLine(fd.linenum.toInt());
		updateFilePathLabel();
	}
	else
	{
		// different file
		//printf("Fileviewer: different file\n");
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(true);
		m_pushButtonNext->setEnabled(false);
		updateTextEdit();
	}
	if ((m_fileDataList.isEmpty())||(m_iter == m_fileDataList.end())) return;
	QVector<filedata>::iterator it = m_fileDataList.begin();
	while ((it != m_fileDataList.end() - 1)&&(it != m_fileDataList.end()))
	{
		if (it->compare(fd)) it = m_fileDataList.erase(it);
		else it++;
	}
	if (m_fileDataList.size() > 20) m_fileDataList.erase(m_fileDataList.begin());
	m_iter = m_fileDataList.end() - 1;
}

void fileviewer::updateTextEdit(void)
{
	if (m_iter == m_fileDataList.end()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	m_textEditSource->clear();

	QFile file(m_iter->filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QApplication::restoreOverrideCursor();
		return;
	}
	QTextStream in(&file);

	int lang = enHighlightCPP; // default

	QRegExp rx1("\\.py$", Qt::CaseInsensitive);
	int pos = rx1.indexIn(m_iter->filename);
	if (pos != -1) lang = enHighlightPython;

	QRegExp rx2("\\.java$", Qt::CaseInsensitive);
	pos = rx2.indexIn(m_iter->filename);
	if (pos != -1) lang = enHighlightJava;

	QRegExp rx3("\\.rb$", Qt::CaseInsensitive);
	pos = rx3.indexIn(m_iter->filename);
	if (pos != -1) lang = enHighlightRuby;

	QRegExp rx4("\\.js$", Qt::CaseInsensitive);
	pos = rx4.indexIn(m_iter->filename);
	if (pos != -1) lang = enHighlightJavascript;

	m_currentlang = lang;
	setLexer(lang);

	QString alltext;
	while (!in.atEnd())
	{
		alltext = in.readAll();
	}
	m_textEditSource->setText(alltext);
	m_textEditSource->setMarginWidth(0,  QString::number(m_textEditSource->lines() * 10));
	highlightLine(m_iter->linenum.toInt());
	updateFilePathLabel();
	m_pushButtonGoToLine->setEnabled(true);
	m_pushButtonOpenInEditor->setEnabled(true);
	m_pushButtonTextShrink->setEnabled(true);
	m_pushButtonTextEnlarge->setEnabled(true);
	m_listWidgetFunc->clear();
	if (m_iter->fileid < 0)	{emit requestFuncList_filename(m_iter->filename);}
	else {emit requestFuncList_fileid(m_iter->fileid);}
	QApplication::restoreOverrideCursor();
}

void fileviewer::updateFilePathLabel(void)
{
	QString labeltext = m_iter->filename;
	labeltext += ":";
	labeltext += m_iter->linenum;
	m_labelFilePath->setText(labeltext);
	m_pushButtonGoToLine->setEnabled(true);
}

void fileviewer::AbleToCopy(bool copy)
{
	int lineFrom, indexFrom, lineTo, indexTo;
	m_pushButtonPaste->setEnabled(copy);
	if (copy)
	{
		m_textEditSource->copy();
		m_textEditSource->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
		m_annotline = lineTo;
		QString str = (QApplication::clipboard())->text();
		m_textEditSource->clearAnnotations();
		if (str.length() > 0)
			emit requestAnnotation(str);
	}
}

void fileviewer::GoToLine_ButtonClick(bool checked)
{
	if (!checked) highlightLine(m_iter->linenum.toInt());
}

void fileviewer::Prev_ButtonClick(bool checked)
{
	if (m_fileDataList.isEmpty())
	{
		m_pushButtonPrev->setEnabled(false);
		m_pushButtonNext->setEnabled(false);
		return;
	}
	if(m_iter == m_fileDataList.begin())
	{
		m_pushButtonPrev->setEnabled(false);
		return;
	}
	if (!checked)
	{
		QVector<filedata>::iterator it = m_iter;
		m_iter--;
		if ((it != m_fileDataList.end())&&(m_iter->compareFilePathOnly(*it)))
		{
			highlightLine(m_iter->linenum.toInt());
			updateFilePathLabel();			
		}
		else
		{
			updateTextEdit();
		}
		m_pushButtonPrev->setEnabled(m_iter != m_fileDataList.begin());
		m_pushButtonNext->setEnabled(m_iter != m_fileDataList.end() - 1);
	}
}

void fileviewer::Next_ButtonClick(bool checked)
{
	if (m_fileDataList.isEmpty())
	{
		m_pushButtonPrev->setEnabled(false);
		m_pushButtonNext->setEnabled(false);
		return;
	}
	if((m_iter == m_fileDataList.end() - 1)||
		(m_iter == m_fileDataList.end()))
	{
		m_pushButtonNext->setEnabled(false);
		return;
	}
	if (!checked)
	{
		QVector<filedata>::iterator it = m_iter;
		m_iter++;
		if (m_iter->compareFilePathOnly(*it))
		{
			highlightLine(m_iter->linenum.toInt());
			updateFilePathLabel();			
		}
		else
		{
			updateTextEdit();
		}
		m_pushButtonPrev->setEnabled(m_iter != m_fileDataList.begin());
		m_pushButtonNext->setEnabled(m_iter != m_fileDataList.end() - 1);
	}
}

void fileviewer::handleFileCannotBeOpenedCase(void)
{
	//printf("handleFileCannotBeOpenedCase\n");
	m_textEditSource->clear();
	m_pushButtonTextShrink->setEnabled(false);
	m_pushButtonTextEnlarge->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);	
	m_pushButtonNext->setEnabled(false);
	m_pushButtonPrev->setEnabled(m_fileDataList.isEmpty() == false);
	m_iter = m_fileDataList.end();
}


void fileviewer::Paste_ButtonClick(bool checked)
{
	if (!checked)
	{
		m_textEditSource->copy();
		if (m_checkBoxSymbolOnly->isChecked())
			emit searchCopiedTextSymbolOnly();
		else emit searchCopiedText();
	}
}

void fileviewer::fileViewSettings_Triggered(bool checked)
{
	cqDialogFileViewSettings cqdg((QWidget*)mw, this,
		m_fontlist, themes::getThemesList());
	m_fonttemp = m_textEditSourceFont.family();
	m_fontwidthtemp = (m_textEditSource->tabWidth());
	m_themetemp = m_theme;
	cqdg.setCurrentFontType(m_fonttemp);
	cqdg.setTabWidth(m_fontwidthtemp);
	cqdg.setCurrentTheme(m_themetemp);
	cqdg.setModal(true);
	cqdg.exec();
	if (cqdg.result() == QDialog::Accepted)
	{
		m_textEditSourceFont.setFamily(m_fonttemp);
		//m_lexer->setFont(m_textEditSourceFont);
		m_textEditSource->setTabWidth(m_fontwidthtemp);
		m_textEditSource->zoomTo(m_fontsize);
		m_theme = m_themetemp;
		m_themelast = "1234";
		setLexer();
	}
}

void fileviewer::OptionsExtEditor_Triggered(bool checked)
{
	bool ok;
	QString inptext;
	QInputDialog qinp(mw);
	qinp.setCancelButtonText(tr("Cancel"));
	qinp.setOkButtonText(tr("OK"));
	qinp.setInputMode(QInputDialog::TextInput);
	qinp.setWindowTitle(tr("External Editor Configuration"));
	QString exted = tr("Please enter the path and arguments for the external editor. Replace as follows:");
	exted += "\n%f - ";
	exted += tr("for file path");
	exted += "\n%n - ";
	exted += tr("for line number");
	exted += "\n";
	exted += tr("For example:");
#ifdef _WIN32
	exted += "\n\"C:\\Program Files\\Notepad++\\notepad++.exe\" -n%n %f";
#else
	exted += "\ngedit %f +%n";
#endif
	qinp.setLabelText(exted);
	qinp.setTextEchoMode(QLineEdit::Normal);
	qinp.setTextValue(m_externalEditorPath);
	qinp.exec();
	ok = (qinp.result() == QDialog::Accepted);
	inptext = qinp.textValue();
	if (ok && (inptext.isEmpty() == false)) m_externalEditorPath = inptext.trimmed();
}

void fileviewer::OpenInEditor_ButtonClick(bool checked)
{
	QMessageBox msgBox(mw);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStandardButtons(QMessageBox::Ok);
	if (!checked)
	{
		QFile file(m_iter->filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			msgBox.setText(tr("File could not be opened!"));
			msgBox.exec();
			return;
		}
		file.close();

		QStringList arguments;
		QString program;
		QRegExp rx("^\"([^\"]+)\" (.*)");
		int pos = rx.indexIn(m_externalEditorPath);
		if (pos != -1)
		{
			program = rx.cap(1);
			arguments = (rx.cap(2)).split(QRegExp("[ ]+"));
		}
		else
		{
			arguments = m_externalEditorPath.split(QRegExp("[ ]+"));
			program = arguments.takeFirst();
		}
		arguments.replaceInStrings(QRegExp("%f"), m_iter->filename);
		arguments.replaceInStrings(QRegExp("%n"), m_iter->linenum);

		if (QProcess::startDetached(program, arguments) == false)
		{
			msgBox.setText(tr("External editor could not be started. Please check Options!"));
			msgBox.exec();
			return;
		}
	}
}

void fileviewer::TextShrink_ButtonClick(bool checked)
{
	if (!checked)
	{
		textSizeChange(0-1);
		//m_textEditSource->zoomOut();
	}
}

void fileviewer::TextEnlarge_ButtonClick(bool checked)
{
	if (!checked)
	{
		textSizeChange(1);
		//m_textEditSource->zoomIn();
	}
}

void fileviewer::textSizeChange(int n)
{
	//m_fontwidthtemp = (m_textEditSource->tabWidth());
	//m_lexer->setFont(m_textEditSourceFont);
	m_fontsize += n;
	m_textEditSource->zoomTo(m_fontsize);
	m_textEditSource->setMarginWidth(0,  QString::number(m_textEditSource->lines() * 10));
	//m_textEditSource->setTabWidth(m_fontwidthtemp);
}

void fileviewer::fontSelectionTemporary(const QString &fonttxt)
{
	m_fonttemp = fonttxt;
}

void fileviewer::themeSelectionTemporary(const QString &themetxt)
{
	m_themetemp = themetxt;
}

void fileviewer::tabWidthSelectionTemporary(const QString &width)
{
	m_fontwidthtemp = width.toInt();
}

void fileviewer::highlightLine(unsigned int num)
{
	m_textEditSource->markerDeleteAll();
	if (num <= 0)
	{
		num = 1;
	}
	else
	{
		num = num - 1; // not sure why it's one off
		m_textEditSource->markerAdd(num, m_markerhandle);
		m_textEditSource->markerAdd(num, m_markerhandle2);
	}
	m_textEditSource->ensureLineVisible(num);
	m_currentline = num;
}

void fileviewer::setLexer(int lang)
{
	if (lang == -1) lang = m_currentlang;
	if (m_lexer == NULL)
	{
		m_lexer = new QsciLexerCPP(m_textEditSource);
		//m_lexer->setFont(m_textEditSourceFont);
		m_textEditSource->setLexer(m_lexer);
		m_textEditSource->zoomTo(m_fontsize);
		m_themelast = "1234";
	}

	switch(lang)
	{

		case enHighlightCPP:
			replaceLexer("C++", lang);
			break;

		case enHighlightPython:
			replaceLexer("Python", lang);
			break;

		case enHighlightJava:
			replaceLexer("Java", lang);
			break;

		case enHighlightRuby:
			replaceLexer("Ruby", lang);
			break;

		case enHighlightJavascript:
			replaceLexer("JavaScript", lang);
			break;

		default:
			replaceLexer("C++", lang);
			break;

	}

}

void fileviewer::replaceLexer(const char* langstr, int lang)
{
	QColor markerlinebgcolor;
	QColor linenumfgcolor;
	if ((strlen(m_lexer->language()) != strlen(langstr)) ||
		(strcmp(m_lexer->language(), langstr) != 0))
	{
		m_textEditSource->setLexer(NULL);
		delete m_lexer;
		switch (lang)
		{
			case enHighlightCPP:
				m_lexer = new QsciLexerCPP(m_textEditSource);
				break;

			case enHighlightPython:
				m_lexer = new QsciLexerPython(m_textEditSource);
				break;

			case enHighlightJava:
				m_lexer = new QsciLexerJava(m_textEditSource);
				break;

			case enHighlightRuby:
				m_lexer = new QsciLexerRuby(m_textEditSource);
				break;

			case enHighlightJavascript:
				m_lexer = new QsciLexerJavaScript(m_textEditSource);
				break;

			default:
				m_lexer = new QsciLexerCPP(m_textEditSource);
				break;
		}
		//m_lexer->setFont(m_textEditSourceFont);
		m_textEditSource->setLexer(m_lexer);
		m_textEditSource->zoomTo(m_fontsize);
		m_themelast = "1234";
	}
	if (m_themelast.compare(m_theme) != 0)
	{
		m_themelast = m_theme;
		themes::setTheme(m_theme, lang, m_lexer, m_textEditSourceFont, markerlinebgcolor, linenumfgcolor);
		m_textEditSource->setMarkerBackgroundColor(markerlinebgcolor, m_markerhandle);
		m_textEditSource->setMarkerBackgroundColor(linenumfgcolor, m_markerhandle2);
		m_textEditSource->setMarginsFont(m_textEditSourceFont);
		//m_textEditSource->setLexer(m_lexer);
		m_textEditSource->zoomTo(m_fontsize);
		m_textEditSource->setMarginWidth(0,  QString::number(m_textEditSource->lines() * 10));
		m_textEditSource->recolor();
	}
}

void fileviewer::annotate(QString annotstr)
{
	m_textEditSource->clearAnnotations();
	m_textEditSource->annotate(m_annotline, annotstr, 29);
}

void fileviewer::recvFuncList(sqlqueryresultlist* reslist)
{
	m_listWidgetFunc->clear();
	if ((m_fileDataList.isEmpty())||(m_iter == m_fileDataList.end())) return;
	m_funclist = *reslist;
	filedata fd(str2qt(m_funclist.resultlist[0].filename), "1", -99);
	if (m_iter->compareFileNameOnly(fd) == false)
	{
		if (m_iter->fileid < 0)	{emit requestFuncList_filename(m_iter->filename);}
		else {emit requestFuncList_fileid(m_iter->fileid);}
		return;
	}
	if (m_comboBoxFuncListSort->currentIndex() == 0)
		{m_funclist.sort_by_linenum();}
	else
		{m_funclist.sort_by_name();}
	for (int i=0; i < m_funclist.resultlist.size(); i++)
	{
		m_listWidgetFunc->addItem(new QListWidgetItem(
			str2qt(m_funclist.resultlist[i].symname), 0, 
			atoi(m_funclist.resultlist[i].linenum.c_str())));
	}
}

void fileviewer::funcItemSelected(QListWidgetItem * curitem, QListWidgetItem * previtem)
{
	if (curitem == NULL) return;
	int num = curitem->type();
	if (num <= 0)
	{
		num = 1;
	}
	else
	{
		num = num - 1; // not sure why it's one off
	}
	m_textEditSource->ensureLineVisible(num);
}

void fileviewer::FuncListSort_indexChanged(const int& idx)
{
	recvFuncList(&m_funclist);
}

