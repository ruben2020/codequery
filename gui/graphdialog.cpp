
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include "ui_graphDialog.h"
#include "graphdialog.h"
#include "showgraph.h"


cqDialogGraph::cqDialogGraph(QWidget *parent)
:QDialog(parent)
,dialog_ui(new Ui::DialogGraph)
,m_scaleFactor(5.0)
 {
	dialog_ui->setupUi(this);
	connect(dialog_ui->pushButtonClose, SIGNAL(clicked()),
		this, SLOT(accept()));
	connect(dialog_ui->pushButtonZoomOut, SIGNAL(clicked()),
		this, SLOT(zoomout()));
	connect(dialog_ui->pushButtonZoomIn, SIGNAL(clicked()),
		this, SLOT(zoomin()));
	connect(dialog_ui->pushButtonSave, SIGNAL(clicked()),
		this, SLOT(savetoimagefile()));
	connect(dialog_ui->pushButtonSaveDot, SIGNAL(clicked()),
		this, SLOT(savetodotfile()));
	connect(dialog_ui->comboBoxNbrOfLevels, SIGNAL(currentIndexChanged(int)),
		this, SLOT(numberOfLevelsChanged(int)));

}

cqDialogGraph::~cqDialogGraph()
 {
	disconnect();
	delete dialog_ui;
 }

void cqDialogGraph::setupGraphFromXML(QStringList& grpxml, QStringList& grpdot, QString& desc)
{
	m_grpxml = grpxml;
	m_grpdot = grpdot;
	m_img = showgraph::convertToImage(grpxml[0]);
	dialog_ui->labelGraph->setPixmap(QPixmap::fromImage(m_img));
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	dialog_ui->labelGraph->setMask(dialog_ui->labelGraph->pixmap(Qt::ReturnByValue).mask());
#else
	dialog_ui->labelGraph->setMask(dialog_ui->labelGraph->pixmap()->mask());
#endif
	if (desc.length() > 0) dialog_ui->labelDesc->setText(desc);
	for (unsigned int i=0; i < grpxml.size(); i++) dialog_ui->comboBoxNbrOfLevels->addItem(QString::number(i+1));
	dialog_ui->comboBoxNbrOfLevels->setCurrentIndex(0);
	show();
	adjustScrollBar(dialog_ui->scrollArea->horizontalScrollBar(), m_scaleFactor/5);
	adjustScrollBar(dialog_ui->scrollArea->verticalScrollBar(), m_scaleFactor/5);
}

void cqDialogGraph::numberOfLevelsChanged(int num)
{
	m_img = showgraph::convertToImage(m_grpxml[dialog_ui->comboBoxNbrOfLevels->currentIndex()]);
	dialog_ui->labelGraph->setPixmap(QPixmap::fromImage(m_img));
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	dialog_ui->labelGraph->setMask(dialog_ui->labelGraph->pixmap(Qt::ReturnByValue).mask());
#else
	dialog_ui->labelGraph->setMask(dialog_ui->labelGraph->pixmap()->mask());
#endif
	adjustScrollBar(dialog_ui->scrollArea->horizontalScrollBar(), m_scaleFactor/5);
	adjustScrollBar(dialog_ui->scrollArea->verticalScrollBar(), m_scaleFactor/5);
}

void cqDialogGraph::zoomout()
{
     scaleImage(-1);
}

void cqDialogGraph::zoomin()
{
     scaleImage(1);
}

void cqDialogGraph::savetoimagefile()
{
	QString filetype = tr("Images");
	filetype += " (*.png *.jpg *.bmp *.tiff)";
	QString fileName =
	QFileDialog::getSaveFileName( this, tr("Export Image"),
					QDir::currentPath(),
					filetype);
	if (fileName.isEmpty()) return;

	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStandardButtons(QMessageBox::Ok);
	QImageWriter writer( fileName);
	if ((writer.canWrite() && writer.write(m_img)) == false)
	{
		msgBox.setText(tr("File could not be saved!"));
		msgBox.exec();
	}
}

void cqDialogGraph::savetodotfile()
{
	QString fileName =
	QFileDialog::getSaveFileName( this, tr("Export DOT file"),
					QDir::currentPath(),
					"Graphviz DOT (*.dot)");
	if (fileName.isEmpty()) return;

	QFile outfile(fileName);
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setStandardButtons(QMessageBox::Ok);
	if (outfile.open(QIODevice::WriteOnly | QIODevice::Text) == false)
	{
		msgBox.setText(tr("File could not be saved!"));
		msgBox.exec();
		return;
	}
	QTextStream out(&outfile);
	out << m_grpdot[dialog_ui->comboBoxNbrOfLevels->currentIndex()];
	outfile.close();
}

void cqDialogGraph::scaleImage(double factor)
{
     QPixmap p = QPixmap::fromImage(m_img);
     m_scaleFactor += factor;
     m_scaleFactor = (m_scaleFactor < 1.0) ? 1.0 : m_scaleFactor;
     dialog_ui->labelGraph->setPixmap(p.scaled((m_scaleFactor/5) * p.size(), Qt::KeepAspectRatio));
     adjustScrollBar(dialog_ui->scrollArea->horizontalScrollBar(), m_scaleFactor/5);
     adjustScrollBar(dialog_ui->scrollArea->verticalScrollBar(), m_scaleFactor/5);
}

void cqDialogGraph::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	int minim = scrollBar->minimum();
	scrollBar->setValue((scrollBar->maximum() - minim)/2 + minim);
}

