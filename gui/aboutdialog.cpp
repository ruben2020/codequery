
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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


