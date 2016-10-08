
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

#include "ui_aboutDialog.h"
#include "aboutdialog.h"
#include "swver.h"


cqDialogAbout::cqDialogAbout(QWidget *parent)
:QDialog(parent)
,dialog_ui(new Ui::aboutDialog)
 {
	dialog_ui->setupUi(this);
	dialog_ui->labelTitle->setText(CODEQUERY_SW_VERSION_WEBSITE);
	dialog_ui->labelLicense->setText(CODEQUERY_SW_LICENSE_PARA_LINK);
	connect(dialog_ui->pushButtonCloseAbout, SIGNAL(clicked()),
		this, SLOT(accept()));
	resize(sizeHint());
	layout()->setSizeConstraint(QLayout::SetFixedSize) ;
	setSizeGripEnabled(false) ;
}

cqDialogAbout::~cqDialogAbout()
 {
	disconnect();
	delete dialog_ui;
 }


