
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef MAINWINDOW_H_CQ
#define MAINWINDOW_H_CQ

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#else
#include <QtGui>
#endif


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
void writeSettings();
void readSettings();

public slots:
void AboutQtTriggered(bool checked);
void AboutTriggered(bool checked);
void ExitTriggered(bool checked);
void LanguageTriggered(bool checked);
void prepareToExit();

signals:
void windowResized();
void windowRepainted();

private:
QApplication *m_app;
fileviewer* m_fileviewer;
listhandler* m_listhandler;
searchhandler* m_searchhandler;
QString m_currentLanguage;
QTranslator m_translator;

void init(void);

protected:
virtual void resizeEvent(QResizeEvent* event);
virtual void paintEvent(QPaintEvent *event);

};

 
 
#endif
 
