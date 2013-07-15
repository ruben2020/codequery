
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

#include "ui_fileViewSettingsDialog.h"
#include "fileviewsettingsdialog.h"
#include "fileviewer.h"
#include <QFontDatabase>

cqDialogFileViewSettings::cqDialogFileViewSettings(QWidget *parent,
		fileviewer* fv, const QStringList& fontlst)
:QDialog(parent)
,m_fv(fv)
,dialog_ui(new Ui::fileViewSettingsDialog)
,m_tabwidthvalidator(1,64)
 {
	dialog_ui->setupUi(this);
	dialog_ui->lineEditTabWidth->setValidator(&m_tabwidthvalidator);
        dialog_ui->comboBoxFont->addItems(fontlst);
	connect(dialog_ui->pushButtonOK, SIGNAL(clicked()),
		this, SLOT(accept()));
	connect(dialog_ui->pushButtonCancel, SIGNAL(clicked()),
		this, SLOT(reject()));
	connect(dialog_ui->comboBoxFont, SIGNAL(currentIndexChanged(const QString &)),
			fv, SLOT(fontSelectionTemporary(const QString &)));
	connect(dialog_ui->lineEditTabWidth, SIGNAL(textEdited(const QString &)),
			fv, SLOT(tabWidthSelectionTemporary(const QString &)));
	resize(sizeHint());
	layout()->setSizeConstraint(QLayout::SetFixedSize) ;
	setSizeGripEnabled(false) ;
}

cqDialogFileViewSettings::~cqDialogFileViewSettings()
 {
	disconnect();
	delete dialog_ui;
 }

void cqDialogFileViewSettings::setCurrentFontType(const QString& fonttype)
{
	int idx = dialog_ui->comboBoxFont->findText(fonttype, Qt::MatchContains);
	dialog_ui->comboBoxFont->setCurrentIndex(idx);
}

void cqDialogFileViewSettings::setTabWidth(const int& width)
{
	dialog_ui->lineEditTabWidth->setText(QString::number(width));
}


