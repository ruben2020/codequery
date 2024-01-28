
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef FILEVIEWSETTINGSDIALOG_H_CQ
#define FILEVIEWSETTINGSDIALOG_H_CQ

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif


 namespace Ui {
     class fileViewSettingsDialog;
 }

class fileviewer;

class cqDialogFileViewSettings : public QDialog
{
  Q_OBJECT

public:
Ui::fileViewSettingsDialog *dialog_ui;

cqDialogFileViewSettings(QWidget *parent, fileviewer* fv,
const QStringList& fontlst, const QStringList& themeslst);
virtual ~cqDialogFileViewSettings();
void setCurrentFontType(const QString& fonttype);
void setCurrentTheme(const QString& theme);
void setTabWidth(const int& width);

public slots:


private:
QIntValidator m_tabwidthvalidator;
fileviewer* m_fv;

};


#endif //FILEVIEWSETTINGSDIALOG_H_CQ

