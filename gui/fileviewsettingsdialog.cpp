
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include "ui_fileViewSettingsDialog.h"
#include "fileviewsettingsdialog.h"
#include "fileviewer.h"
#include <QFontDatabase>

cqDialogFileViewSettings::cqDialogFileViewSettings(QWidget *parent,
		fileviewer* fv, const QStringList& fontlst, const QStringList& themeslst)
:QDialog(parent)
,m_fv(fv)
,dialog_ui(new Ui::fileViewSettingsDialog)
,m_tabwidthvalidator(1,64)
 {
	dialog_ui->setupUi(this);
	dialog_ui->lineEditTabWidth->setValidator(&m_tabwidthvalidator);
        dialog_ui->comboBoxFont->addItems(fontlst);
        dialog_ui->comboBoxTheme->addItems(themeslst);
	connect(dialog_ui->pushButtonOK, SIGNAL(clicked()),
		this, SLOT(accept()));
	connect(dialog_ui->pushButtonCancel, SIGNAL(clicked()),
		this, SLOT(reject()));
	connect(dialog_ui->comboBoxFont, SIGNAL(currentIndexChanged(int)),
		this, SLOT(fontSelectionTemporary(int)));
	connect(dialog_ui->comboBoxTheme, SIGNAL(currentIndexChanged(int)),
		this, SLOT(themeSelectionTemporary(int)));
	connect(dialog_ui->lineEditTabWidth, SIGNAL(textEdited(const QString &)),
			fv, SLOT(tabWidthSelectionTemporary(const QString &)));
	connect(this, SIGNAL(fontSelectionChanged(const QString &)),
		fv, SLOT(fontSelectionTemporary(const QString &)));
	connect(this, SIGNAL(themeSelectionChanged(const QString &)),
		fv, SLOT(themeSelectionTemporary(const QString &)));
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

void cqDialogFileViewSettings::setCurrentTheme(const QString& theme)
{
	int idx = dialog_ui->comboBoxTheme->findText(theme, Qt::MatchContains);
	dialog_ui->comboBoxTheme->setCurrentIndex(idx);
}

void cqDialogFileViewSettings::setTabWidth(const int& width)
{
	dialog_ui->lineEditTabWidth->setText(QString::number(width));
}

void cqDialogFileViewSettings::fontSelectionTemporary(int index)
{
	emit fontSelectionChanged(dialog_ui->comboBoxFont->itemText(index));
}

void cqDialogFileViewSettings::themeSelectionTemporary(int index)
{
	emit themeSelectionChanged(dialog_ui->comboBoxTheme->itemText(index));
}

