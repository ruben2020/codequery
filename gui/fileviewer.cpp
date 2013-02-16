
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


#include <QProcess>
#include <QInputDialog>

#include "fileviewer.h"
#include "mainwindow.h"
#include "CodeEditor.h"
#include "highlighter.h"

#ifdef _WIN32
#define EXT_EDITOR_DEFAULT_PATH "notepad %f"
#else
#define EXT_EDITOR_DEFAULT_PATH "gedit %f +%n"
#endif

filedata::filedata()
{
	linenum = "1";
}
filedata::filedata(const QString& fn, const QString& ln)
{
	linenum = ln;
	filename = fn;
}

bool filedata::compare(const filedata& fd)
{
	return ((linenum.compare(fd.linenum) == 0)&&
			(filename.compare(fd.filename) == 0));
}

bool filedata::compareFilenameOnly(const filedata& fd)
{
	return (filename.compare(fd.filename) == 0);
}

filedata::filedata(const filedata& fd)
{
	linenum = fd.linenum;
	filename = fd.filename;
}

filedata& filedata::operator=(const filedata& fd)
{
	if (&fd != this)
	{
		linenum = fd.linenum;
		filename = fd.filename;
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
,m_highlighter(NULL)
,m_textEditSourceFont("Courier New")
,m_externalEditorPath(EXT_EDITOR_DEFAULT_PATH)
{
	m_iter = m_fileDataList.begin();
	m_textEditSourceFont.setStyleHint(QFont::TypeWriter);	
}

fileviewer::~fileviewer()
{
	if (m_highlighter != NULL) delete m_highlighter;
}

void fileviewer::init(void)
{
	m_pushButtonPaste->setEnabled(false);
	m_pushButtonPrev->setEnabled(false);
	m_pushButtonNext->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);
	m_pushButtonOpenInEditor->setEnabled(false);
	m_labelFilePath->clear();
	m_textEditSource->clear();
	m_textEditSource->setFont(m_textEditSourceFont);
	m_textEditSource->setWordWrapMode(QTextOption::NoWrap);
	m_textEditSource->setReadOnly(true);
	m_textEditSource->setCenterOnScroll(true);
	m_highlighter = new Highlighter(m_textEditSource->document()); 
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
	m_fileDataList.clear();
}

void fileviewer::clearList()
{
	m_pushButtonPaste->setEnabled(false);
	m_pushButtonPrev->setEnabled(false);
	m_pushButtonNext->setEnabled(false);
	m_pushButtonGoToLine->setEnabled(false);
	m_pushButtonOpenInEditor->setEnabled(false);
	m_labelFilePath->clear();
	m_textEditSource->clear();
	m_fileDataList.clear();
	m_iter = m_fileDataList.begin();
}

void fileviewer::fileToBeOpened(QString filename, QString linenum)
{
	filename.replace(QString("$HOME"), QDir::homePath());
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

	filedata fd(filename, linenum);
	if (m_fileDataList.isEmpty())
	{
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
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(m_iter != m_fileDataList.begin());
		m_pushButtonNext->setEnabled(false);
		updateTextEdit();		
	}
	else if (m_iter->compare(fd))
	{
		// the same filename and line number
		return;
	}
	else if (m_iter->compareFilenameOnly(fd))
	{
		// same file, different line number
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(true);
		m_pushButtonNext->setEnabled(false);
		m_textEditSource->highlightLine(fd.linenum.toInt());
		updateFilePathLabel();
	}
	else
	{
		// different file
		m_fileDataList.push_back(fd);
		m_iter = m_fileDataList.end() - 1;
		m_pushButtonPrev->setEnabled(true);
		m_pushButtonNext->setEnabled(false);
		updateTextEdit();
	}
	QVector<filedata>::iterator it = m_fileDataList.begin();
	while ((it != m_fileDataList.end() - 1)&&(it != m_fileDataList.end()))
	{
		if (it->compare(fd)) m_fileDataList.erase(it);
		else it++;
	}
	if (m_fileDataList.size() > 20) m_fileDataList.erase(m_fileDataList.begin());
}

void fileviewer::updateTextEdit(void)
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	m_textEditSource->clear();
        m_highlighter->m_intAddlRulesMode = 0; //reset additional rules mode

	QFile file(m_iter->filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QApplication::restoreOverrideCursor();
		return;
	}
	QTextStream in(&file);
	while (!in.atEnd())
	{
		m_textEditSource->insertPlainText(in.readAll());
	}
	m_textEditSource->highlightLine(m_iter->linenum.toInt());
	updateFilePathLabel();
	m_pushButtonGoToLine->setEnabled(true);
	m_pushButtonOpenInEditor->setEnabled(true);
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
	m_pushButtonPaste->setEnabled(copy);
}

void fileviewer::GoToLine_ButtonClick(bool checked)
{
	if (!checked) m_textEditSource->highlightLine();
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
		if ((it != m_fileDataList.end())&&(m_iter->compareFilenameOnly(*it)))
		{
			m_textEditSource->highlightLine(m_iter->linenum.toInt());
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
		if (m_iter->compareFilenameOnly(*it))
		{
			m_textEditSource->highlightLine(m_iter->linenum.toInt());
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
	m_textEditSource->clear();
        m_highlighter->m_intAddlRulesMode = 0; //reset additional rules mode
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

void fileviewer::OptionsExtEditor_Triggered(bool checked)
{
	bool ok;
	QString inptext;
	QInputDialog qinp(mw);
	qinp.setCancelButtonText(tr("Cancel"));
	qinp.setOkButtonText(tr("OK"));
	qinp.setInputMode(QInputDialog::TextInput);
	qinp.setWindowTitle(tr("External Editor Configuration"));
	qinp.setLabelText(tr("Please enter the path and arguments for the external editor. "
		"Use %f for filename and %n for line number."
		"For example, gedit %f +%n"));
	qinp.setTextEchoMode(QLineEdit::Normal);
	qinp.setTextValue(m_externalEditorPath);
	qinp.exec();
	ok = (qinp.result() == QDialog::Accepted);
	inptext = qinp.textValue();
	if (ok && (inptext.isEmpty() == false)) m_externalEditorPath = inptext;
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

		QStringList arguments = m_externalEditorPath.split(QRegExp("[ ]+"));
		QString program = arguments.takeFirst();
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


