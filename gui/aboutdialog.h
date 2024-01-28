
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef ABOUTDIALOG_H_CQ
#define ABOUTDIALOG_H_CQ

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif


 namespace Ui {
     class aboutDialog;
 }

class cqDialogAbout : public QDialog
{
  Q_OBJECT

public:
Ui::aboutDialog *dialog_ui;
cqDialogAbout(QWidget *parent);
virtual ~cqDialogAbout();


};


#endif //ABOUTDIALOG_H_CQ


