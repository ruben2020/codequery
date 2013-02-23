/**
 * @file: style_edit.h
 * Interface of style-manipulation widgets
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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