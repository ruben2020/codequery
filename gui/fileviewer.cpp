
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <stdlib.h>
#include <string.h>
#include <QProcess>
#include <QInputDialog>
#include <QFontDatabase>
#include <QFontMetrics>

#include "ScintillaEdit.h"
#include "SciLexer.h"

#include "fileviewer.h"
#include "mainwindow.h"
#include "fileviewsettingsdialog.h"
#include "themes.h"

#if defined(_WIN32)
#define EXT_EDITOR_DEFAULT_PATH "notepad %f"
#elif defined(__APPLE__)
#define EXT_EDITOR_DEFAULT_PATH "open -t %f"
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
		extract_filename(filename.C_STR()),
		extract_filename(fd.filename.C_STR())) == 0);
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
,m_lexer(SCLEX_NULL)
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

QString fileviewer::checkFontFamily(QString fontname)
{
	QString newfont;
#ifdef _WIN32
	QString tryfont1 = "Consolas";
	QString tryfont2 = "Courier New";
#else
	QString tryfont1 = "Monospace";
	QString tryfont2 = "Ubuntu Mono";
#endif
	if (m_fontlist.isEmpty()) createFontList();
	if (m_fontlist.contains(fontname))
	{
		newfont = fontname;
	}
	else
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
		newfont = QFontDatabase::systemFont(QFontDatabase::FixedFont).family();
#else
		if      (m_fontlist.contains(tryfont1)) newfont = tryfont1;
		else if (m_fontlist.contains(tryfont2)) newfont = tryfont2;
		else newfont = m_fontlist[0];
#endif
	}
	return newfont;
}

void fileviewer::init(void)
{
	Scintilla_LinkLexers();
	m_pushButtonPaste->setEnabled(false);
	m_pushButtonPrev->setEnabled(false);
	m_pushButtonNext->setEnabled(false);
	m_pushButtonTextShrink->setEnabled(false);
	m_pushButtonTextEnlarge->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);
	m_pushButtonOpenInEditor->setEnabled(false);
	m_labelFilePath->clear();
	m_textEditSource->setWrapMode(SC_WRAP_NONE);
	m_textEditSource->setReadOnly(true);
	m_markerhandle = 0;
	m_markerhandle2 = 1;
	m_textEditSource->markerDefine(m_markerhandle, SC_MARK_BACKGROUND);
	m_textEditSource->markerDefine(m_markerhandle2, SC_MARK_ARROW);
	m_textEditSource->setMarginTypeN(0, SC_MARGIN_NUMBER);
	m_textEditSource->setMarginTypeN(1, SC_MARGIN_SYMBOL);
	m_textEditSource->annotationSetVisible(ANNOTATION_BOXED);
	m_textEditSource->setCodePage(SC_CP_UTF8);
	m_textEditSource->setCaretPeriod(0);
	createFontList();
	ScintillaEditBase *textEditSourceBase = m_textEditSource;
	connect(textEditSourceBase, SIGNAL(selectionChanged(bool)),
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
	connect(m_listWidgetFunc, SIGNAL(itemPressed(QListWidgetItem *)),
			this, SLOT(funcItemSelected(QListWidgetItem *)));
	connect(m_comboBoxFuncListSort, SIGNAL(currentIndexChanged(int)),
			this, SLOT(FuncListSort_indexChanged(int)));
	m_fileDataList.clear();
	clearTextEdit();
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
	clearTextEdit();
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
		setFilePathLabelText(tr("File not found"));
		handleFileCannotBeOpenedCase();
		return;
	}
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setFilePathLabelText(tr("File could not be opened"));
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

void fileviewer::clearTextEdit(void)
{
	m_textEditSource->setReadOnly(false);
	m_textEditSource->clearAll();
	m_textEditSource->setReadOnly(true);
}

void fileviewer::updateTextEdit(void)
{
	if (m_iter == m_fileDataList.end()) return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	clearTextEdit();

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

	QRegExp rx5("\\.go$", Qt::CaseInsensitive);
	pos = rx5.indexIn(m_iter->filename);
	if (pos != -1) lang = enHighlightGo;

	m_currentlang = lang;

	QString alltext;
	while (!in.atEnd())
	{
		alltext = in.readAll();
	}
	m_textEditSource->setReadOnly(false);
	m_textEditSource->setText(alltext.toUtf8().data());
	m_textEditSource->setReadOnly(true);
	m_textEditSource->setMarginWidthN(0,  m_textEditSource->textWidth(STYLE_LINENUMBER, QString::number(m_textEditSource->lineCount() * 10).C_STR()));
	highlightLine(m_iter->linenum.toInt());
	updateFilePathLabel();
	m_pushButtonGoToLine->setEnabled(true);
	m_pushButtonOpenInEditor->setEnabled(true);
	m_pushButtonTextShrink->setEnabled(true);
	m_pushButtonTextEnlarge->setEnabled(true);
	m_listWidgetFunc->clear();
	if (m_iter->fileid < 0)	{emit requestFuncList_filename(m_iter->filename);}
	else {emit requestFuncList_fileid(m_iter->fileid);}
	setLexer(lang);
	QApplication::restoreOverrideCursor();
}

void fileviewer::updateFilePathLabel(void)
{
	QString labeltext = m_iter->filename;
	labeltext += ":";
	labeltext += m_iter->linenum;
	setFilePathLabelText(labeltext);
	m_pushButtonGoToLine->setEnabled(true);
}

void fileviewer::setFilePathLabelText(QString text)
{
	m_filepathlabeltextfull = text.simplified();
	filePathLabelTextResized();
	repaintWidget();
}

void fileviewer::filePathLabelTextResized()
{
	QFontMetrics metrics(m_labelFilePath->font());
	QString elidedText = metrics.elidedText(m_filepathlabeltextfull, Qt::ElideLeft, m_labelFilePath->width());
	m_labelFilePath->setText(elidedText);
}

void fileviewer::repaintWidget()
{
	QPalette palette = m_labelFilePath->palette();
	palette.setColor(QPalette::Base, palette.color(QPalette::Window));
	m_labelFilePath->setPalette(palette);
}

void fileviewer::braceMatchCheck(void)
{
	long cpos, matchpos;
	cpos     = m_textEditSource->currentPos();
	matchpos = m_textEditSource->braceMatch(cpos, 0);
	if (matchpos == -1)
	{
		cpos--;
		matchpos = m_textEditSource->braceMatch(cpos, 0);
	}
	if (matchpos != -1) m_textEditSource->braceHighlight(cpos, matchpos);
	else m_textEditSource->braceHighlight(-1, -1);
}

void fileviewer::updateFuncList(void)
{
	if (m_funclist.resultlist.empty()) return;
	long line = m_textEditSource->lineFromPosition(m_textEditSource->currentPos()) + 1;
	tStr selectedfunc = "";
	unsigned int selectedline, line1;
	auto previt = m_funclist.resultlist.begin();
	for (auto it = m_funclist.resultlist.begin(); it != m_funclist.resultlist.end(); it++)
	{
		if ( (line < it->intLinenum) && (line >= previt->intLinenum) )
		{
			selectedfunc = previt->symname;
			selectedline = previt->intLinenum;
			break;
		}
		previt = it;
	}
	if (selectedfunc.isEmpty() == false)
	{
		auto itemlist = m_listWidgetFunc->findItems(selectedfunc, Qt::MatchExactly);
		for (auto it = itemlist.begin(); it != itemlist.end(); it++)
		{
			line1 = (*it)->data(Qt::UserRole).toUInt();
			if (selectedline == line1)
			{
				m_listWidgetFunc->setCurrentItem(*it);
				break;
			}
		}
	}
}

void fileviewer::AbleToCopy(bool copy)
{
	m_pushButtonPaste->setEnabled(copy);
	m_textEditSource->annotationClearAll();
	braceMatchCheck();
	updateFuncList();
	if (copy)
	{
		m_textEditSource->copy();
		m_annotline = m_textEditSource->lineFromPosition(m_textEditSource->selectionEnd());
		QString str = (QApplication::clipboard())->text();
		if (str.length() > 0)
		{
			emit requestAnnotation(str);
		}
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
	clearTextEdit();
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
		m_textEditSource->setZoom(m_fontsize);
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
	m_textEditSource->setZoom(m_fontsize);
	m_textEditSource->setMarginWidthN(0,  m_textEditSource->textWidth(STYLE_LINENUMBER, QString::number(m_textEditSource->lineCount() * 10).C_STR()));
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
	m_textEditSource->markerDeleteAll(-1);
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
	m_textEditSource->setFirstVisibleLine(num);
	m_currentline = num;
}

void fileviewer::setLexer(int lang)
{
	if (lang == -1) lang = m_currentlang;

	switch(lang)
	{

		case enHighlightCPP:
			replaceLexer(SCLEX_CPP, lang);
			break;

		case enHighlightPython:
			replaceLexer(SCLEX_PYTHON, lang);
			break;

		case enHighlightJava:
			replaceLexer(SCLEX_CPP, lang);
			break;

		case enHighlightRuby:
			replaceLexer(SCLEX_RUBY, lang);
			break;

		case enHighlightJavascript:
			replaceLexer(SCLEX_CPP, lang);
			break;

		default:
			replaceLexer(SCLEX_CPP, lang);
			break;

	}
}

void fileviewer::replaceLexer(int sclang, int lang)
{
	QColor markerlinebgcolor;
	QColor linenumfgcolor;
	//if (m_lexer != sclang)
	{
		switch (lang)
		{
			case enHighlightCPP:
				m_lexer = SCLEX_CPP;
				break;

			case enHighlightPython:
				m_lexer = SCLEX_PYTHON;
				break;

			case enHighlightJava:
				m_lexer = SCLEX_CPP;
				break;

			case enHighlightRuby:
				m_lexer = SCLEX_RUBY;
				break;

			case enHighlightJavascript:
				m_lexer = SCLEX_CPP;
				break;

			default:
				m_lexer = SCLEX_CPP;
				break;
		}
		m_textEditSource->setLexer(m_lexer);
		m_textEditSource->clearDocumentStyle();
		m_textEditSource->setZoom(m_fontsize);
		m_themelast = "1234";
	}
	//if (m_themelast.compare(m_theme) != 0)
	{
		m_themelast = m_theme;
		themes::setTheme(m_theme, lang, m_textEditSource, m_textEditSourceFont, markerlinebgcolor, linenumfgcolor);
		m_textEditSource->markerSetBack(m_markerhandle, themes::QC2SC(markerlinebgcolor));
		m_textEditSource->markerSetBack(m_markerhandle2, themes::QC2SC(linenumfgcolor));
		m_textEditSource->markerSetAlpha(m_markerhandle, SC_ALPHA_NOALPHA);
		m_textEditSource->markerSetAlpha(m_markerhandle2, SC_ALPHA_NOALPHA);
		m_textEditSource->styleSetFont(STYLE_LINENUMBER, m_textEditSourceFont.family().C_STR());
		m_textEditSource->setZoom(m_fontsize);
		m_textEditSource->setMarginWidthN(0, m_textEditSource->textWidth(STYLE_LINENUMBER, QString::number(m_textEditSource->lineCount() * 10).C_STR()));
		themes::setKeywords(lang, m_textEditSource);
		m_textEditSource->colourise(0, -1);
	}
}

void fileviewer::annotate(QStringList annotstrLst)
{
	m_textEditSource->copy();
	QString str = (QApplication::clipboard())->text();
	m_annotline = m_textEditSource->lineFromPosition(m_textEditSource->selectionEnd());
	if ((annotstrLst.length() >= 2)&&(annotstrLst[0] == str))
	{
		m_textEditSource->annotationClearAll();
		m_textEditSource->annotationSetText(m_annotline, annotstrLst[1].toUtf8().data());
		m_textEditSource->annotationSetStyle(m_annotline, 29);
	}
	else if (str.length() > 0)
	{
		emit requestAnnotation(str);
	}
}

void fileviewer::recvFuncList(sqlqueryresultlist* reslist)
{
	m_listWidgetFunc->clear();
	if ((m_fileDataList.isEmpty())||(m_iter == m_fileDataList.end())) return;
	m_funclist = *reslist;
	m_funclist.sort_by_linenum();
	for (auto it = m_funclist.resultlist.begin(); it != m_funclist.resultlist.end(); it++)
	{
		it->intLinenum = atoi(it->linenum.C_STR());
	}
	filedata fd(m_funclist.resultlist[0].filename, "1", -99);
	if (m_iter->compareFileNameOnly(fd) == false)
	{
		if (m_iter->fileid < 0)	{emit requestFuncList_filename(m_iter->filename);}
		else {emit requestFuncList_fileid(m_iter->fileid);}
		return;
	}
	sqlqueryresultlist templist = m_funclist;
	if (m_comboBoxFuncListSort->currentIndex() != 0) templist.sort_by_name();
	QListWidgetItem* item;
	for (auto it = templist.resultlist.begin(); it != templist.resultlist.end(); it++)
	{
		item = new QListWidgetItem(it->symname);
		item->setData(Qt::UserRole, QVariant(it->intLinenum));
		m_listWidgetFunc->addItem(item);
	}
}

void fileviewer::funcItemSelected(QListWidgetItem * curitem)
{
	if (curitem == nullptr) return;
	unsigned int num = curitem->data(Qt::UserRole).toUInt();
	m_textEditSource->setFirstVisibleLine(num - 1);
}

void fileviewer::FuncListSort_indexChanged(const int& idx)
{
	recvFuncList(&m_funclist);
}

