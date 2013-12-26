
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
	/*connect(dialog_ui->checkBoxAutoResize, SIGNAL(stateChanged(int)),
		this, SLOT(autoResizeChanged(int)));*/
	connect(dialog_ui->pushButtonSave, SIGNAL(clicked()),
		this, SLOT(savetoimagefile()));
	connect(dialog_ui->pushButtonSaveDot, SIGNAL(clicked()),
		this, SLOT(savetodotfile()));

}

cqDialogGraph::~cqDialogGraph()
 {
	disconnect();
	delete dialog_ui;
 }

void cqDialogGraph::setupGraphFromXML(QString grpxml, QString grpdot, QString desc)
{
	showgraph sg;
	m_dot = grpdot;
	m_img = sg.convertToImage(grpxml);
	dialog_ui->labelGraph->setPixmap(QPixmap::fromImage(m_img));
	dialog_ui->labelGraph->setMask(dialog_ui->labelGraph->pixmap()->mask());
	dialog_ui->labelDesc->setText(desc);
	show();
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
	out << m_dot;
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

/*
void cqDialogGraph::autoResizeChanged(int resizestate)
{
	dialog_ui->scrollArea->setWidgetResizable(resizestate == Qt::Checked);
}*/


