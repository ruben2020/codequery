
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


#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QtGui>

 namespace Ui {
     class MainWindow;
 }

class fileviewer;
class listhandler;
class searchhandler;

class mainwindow : public QMainWindow
{
  Q_OBJECT

public:
Ui::MainWindow *ui;
mainwindow(QMainWindow *parent = NULL, QApplication *app = NULL);
virtual ~mainwindow();
void setup_fileviewer(void);
void setup_listhandler(void);
void setup_searchhandler(void);
void retranslateUi(void);

public slots:
void ExitTriggered(bool checked);
void LanguageTriggered(bool checked);
void prepareToExit();

private:
QApplication *m_app;
fileviewer* m_fileviewer;
listhandler* m_listhandler;
searchhandler* m_searchhandler;
QString m_currentLanguage;
QTranslator m_translator;

void init(void);

};

 
 
#endif
 
