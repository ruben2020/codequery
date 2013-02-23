/**
 * @file: style_edit.cpp 
 * Widgets for style manipulation
 */
/* 
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gview_impl.h"

ColorButton::ColorButton( QWidget *parent):
    QAbstractButton( parent)
{

}

void ColorButton::paintEvent( QPaintEvent *event)
{
    QPainter painter( this);
    painter.setPen( palette().foreground().color());
    painter.setBrush( color);
    painter.drawRect( rect());
}

QSize ColorButton::sizeHint() const
{
    return QSize( 20,20);
}

StyleEdit::StyleEdit( QWidget *parent, bool show_additional)
{
    QColor color;
    if ( show_additional)
    {        
        shape_combo = new QComboBox( this);
        shape_combo->addItem(tr("Box"), NODE_SHAPE_BOX);
        shape_combo->addItem(tr("Rounded box"), NODE_SHAPE_ROUNDED_BOX);
        shape_combo->addItem(tr("Ellipse"), NODE_SHAPE_ELLIPSE);
        shape_combo->addItem(tr("Circle"), NODE_SHAPE_CIRCLE);
        shape_combo->addItem(tr("Diamond"), NODE_SHAPE_DIAMOND);

        shape_label = new QLabel( "Shape:", this);
        shape_label->setBuddy( shape_combo);
    } else
    {
        shape_combo = NULL;
    }

    color = QColor(palette().foreground().color());
    line_color_button = new ColorButton( this);
    line_color_button->setColor( color);
    
    line_color_label = new QLabel( "Line color:", this);
    line_color_label->setBuddy( line_color_button);

    line_style_combo = new QComboBox( this);
    line_style_combo->addItem(tr("Solid"), Qt::SolidLine);
    line_style_combo->addItem(tr("Dash"), Qt::DashLine);
    line_style_combo->addItem(tr("Dot"), Qt::DotLine);
    line_style_combo->addItem(tr("Dash Dot"), Qt::DashDotLine);
    line_style_combo->addItem(tr("Dash Dot Dot"), Qt::DashDotDotLine);
    line_style_combo->addItem(tr("None"), Qt::NoPen);

    line_style_label = new QLabel( "Line style:", this);
    line_style_label->setBuddy( line_style_combo);
    
    line_width_spin = new QDoubleSpinBox( this);
    line_width_spin->setMaximum( 12);
    
    line_width_label = new QLabel( "Line width:", this);
    line_width_label->setBuddy( line_width_spin);

    if ( show_additional)
    {
        color = QColor(palette().base().color());
        fill_check = new QCheckBox( "Fill", this);
        fill_color_button = new ColorButton( this);
        fill_color_button->setColor( color);
        
        fill_color_label = new QLabel( "Fill color:", this);
        fill_color_label->setBuddy( fill_color_button);
    } else
    {
        fill_color_button = NULL;
        fill_color_label = NULL;
        fill_check = NULL;
    }


    ok = new QPushButton( "&OK", this);
    cancel = new QPushButton( "&Cancel", this);

    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->setColumnStretch(1, 3);
    
    //Row 0 name
    if ( show_additional)
    {
        mainLayout->addWidget( shape_label, 0, 0, Qt::AlignRight);
        mainLayout->addWidget( shape_combo, 0, 1);
    }
    //Row 1 line color
    mainLayout->addWidget( line_color_label, 1, 0, Qt::AlignRight);
    mainLayout->addWidget( line_color_button, 1, 1);
    //Row 2 line style
    mainLayout->addWidget( line_style_label, 2, 0, Qt::AlignRight);
    mainLayout->addWidget( line_style_combo, 2, 1);
     //Row 3 line width
    mainLayout->addWidget( line_width_label, 3, 0, Qt::AlignRight);
    mainLayout->addWidget( line_width_spin, 3, 1);
    //Row 4 fill color
    if ( show_additional)
    {
        mainLayout->addWidget( fill_check, 4, 0, 1, 2);
        mainLayout->addWidget( fill_color_label, 5, 0, Qt::AlignRight);
        mainLayout->addWidget( fill_color_button, 5, 1);
    }
    // Row 5 ok & cancel
    mainLayout->addWidget( ok, 6, 0);
    mainLayout->addWidget( cancel, 6, 1);
    
    setLayout( mainLayout);

    if ( show_additional)
    {
        connect( shape_combo, SIGNAL( currentIndexChanged( int)), this, SLOT( changeShape()) );
        connect( fill_color_button, SIGNAL( clicked()), this, SLOT( selectFillColor()) );
        connect( fill_check, SIGNAL( clicked()), this, SLOT( changeFillStyle()) );
    }
    connect( line_color_button, SIGNAL( clicked()), this, SLOT( selectLineColor()) );
    connect( line_style_combo, SIGNAL( currentIndexChanged( int)), this, SLOT( changeLineStyle()) );
    connect( line_width_spin, SIGNAL( valueChanged( double)), this, SLOT( changeLineWidth( double)) );
    

    connect( ok, SIGNAL( clicked()), this, SLOT( accept()));
    connect( cancel, SIGNAL( clicked()), this, SLOT( reject()));

    setWindowTitle( "Edit style");
    setWindowFlags( windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

/** Change name of the current style */
void StyleEdit::changeStyleName()
{

}

/** Invoke color selection for line */
void StyleEdit::selectLineColor()
{
    QColor color = QColorDialog::getColor();
    line_color_button->setColor( color);
    gstyle->setPenColor( color);
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line style */
void StyleEdit::changeLineStyle()
{
    gstyle->setPenStyle((Qt::PenStyle)line_style_combo->itemData( line_style_combo->currentIndex()).toInt());
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line style */
void StyleEdit::changeShape()
{
    emit styleChanged( gstyle);
    gstyle->setShape((NodeShape)shape_combo->itemData( shape_combo->currentIndex()).toInt());
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line width */
void StyleEdit::changeLineWidth( double width)
{
    gstyle->setPenWidth(line_width_spin->value());
    gstyle->setState();
    emit styleChanged( gstyle);
}
/** Set fill style */
void StyleEdit::changeFillStyle()
{
    if ( !fill_check->isChecked())
    { 
        QBrush br = gstyle->brush();
        br.setStyle( Qt::NoBrush);
        gstyle->setBrush( br);
    } else
    {
        QBrush br = gstyle->brush();
        br.setStyle( Qt::SolidPattern);
        gstyle->setBrush( br);
        gstyle->setState();
    }
    emit styleChanged( gstyle);
}
/** Invoke color selection for fill */
void StyleEdit::selectFillColor()
{
    QColor color = QColorDialog::getColor();
    QBrush brush( color);
    fill_color_button->setColor( color);
    gstyle->setBrush( brush);
    gstyle->setState();
    fill_check->setChecked( true);
    emit styleChanged( gstyle);
}

/** Set style */
void StyleEdit::setGStyle( GStyle *st)
{
    gstyle = st;
    QColor color( st->pen().color());
    line_color_button->setColor( color);
    if ( isNotNullP( shape_combo))
    {
        color =  QColor( st->brush().color());
        shape_combo->setCurrentIndex( shape_combo->findData( gstyle->shape()));
        fill_color_button->setColor( color);
        fill_check->setChecked( st->brush().style() != Qt::NoBrush);
        if ( st->brush().style() == Qt::NoBrush)
        {
            QBrush br = gstyle->brush();
            color = QColor( "White");
            br.setColor( color);
            gstyle->setBrush( br);
            fill_color_button->setColor( color);
        }
    }
    line_style_combo->setCurrentIndex( line_style_combo->findData( gstyle->pen().style()));
    line_width_spin->setValue( gstyle->pen().widthF());
}