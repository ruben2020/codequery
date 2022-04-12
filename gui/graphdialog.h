
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef GRAPHDIALOG_H_CQ
#define GRAPHDIALOG_H_CQ

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#else
#include <QtGui>
#endif


 namespace Ui {
     class DialogGraph;
 }

class cqDialogGraph : public QDialog
{
  Q_OBJECT

public:
Ui::DialogGraph *dialog_ui;
cqDialogGraph(QWidget *parent);
virtual ~cqDialogGraph();
void setupGraphFromXML(QStringList& grpxml, QStringList& grpdot, QString& desc);
void scaleImage(double factor);
void adjustScrollBar(QScrollBar *scrollBar, double factor);

public slots:
void zoomout();
void zoomin();
void savetoimagefile();
void savetodotfile();
void numberOfLevelsChanged(int num);

private:
double m_scaleFactor;
QImage m_img;
QString m_dot;
QStringList m_grpxml;
QStringList m_grpdot;

};


#endif //GRAPHDIALOG_H_CQ


