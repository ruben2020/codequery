
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

#ifndef FILEVIEWSETTINGSDIALOG_H_CQ
#define FILEVIEWSETTINGSDIALOG_H_CQ

#ifdef USE_QT5
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

