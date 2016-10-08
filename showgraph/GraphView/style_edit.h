/**
 * @file: style_edit.h
 * Interface of style-manipulation widgets
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#ifndef STYLE_EDIT_H
#define STYLE_EDIT_H

#include "gview_impl.h"
/**
 * Colored button
 */
class ColorButton: public QAbstractButton
{
public:
    ColorButton( QWidget *parent = 0);    /** Constructor */
    void paintEvent( QPaintEvent *event); /** Paint event handler reimplementation */
    QSize sizeHint() const;               /** Size hint reimplementation */
    
    /** Change the button's color */
    void setColor( QColor &cl)            
    {
        color = cl;
    }
private:
    /** Color of the button */
    QColor color;
};

/**
 * Widget for creating/editing styles
 */
class StyleEdit: public QDialog
{
    Q_OBJECT;
public:
    /** Constructor */
    StyleEdit( QWidget *parent = 0, bool show_additional = false);
    /** Set style */
    void setGStyle( GStyle *);
signals:
    /** Lets everyone know that the style has changed and affected objects should be redrwan */
    void styleChanged( GStyle *style);

public slots:
    /** Change name of the current style */
    void changeStyleName();
    /** Invoke color selection for line */
    void selectLineColor();
    /** Change line style */
    void changeLineStyle();
    /** Change line style */
    void changeShape();
    /** Change line width */
    void changeLineWidth( double width);
    /** Invoke color selection for fill */
    void selectFillColor();
    /** Change fill style */
    void changeFillStyle();
private:
    GStyle *gstyle;
    //QHash< QString, GStyle *> styles;
    QLabel *name_label;
    QLabel *line_color_label;
    QLabel *line_style_label;
    QLabel *line_width_label;
    QLabel *fill_color_label;
    QLabel *shape_label;

    QComboBox *name_combo;
    QCheckBox *fill_check; 
    QComboBox *shape_combo;
    ColorButton *line_color_button;
    QComboBox *line_style_combo;
    QDoubleSpinBox *line_width_spin;
    ColorButton *fill_color_button;
    QPushButton *ok;
    QPushButton *cancel;
};

#endif