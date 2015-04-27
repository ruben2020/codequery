
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

#ifndef ABOUTDIALOG_H_CQ
#define ABOUTDIALOG_H_CQ

#ifdef USE_QT5
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


