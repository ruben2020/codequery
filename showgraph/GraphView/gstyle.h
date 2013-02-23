/**
 * @file: gstyle.h 
 * Node/Edge styles in graph
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GSTYLE_H
#define GSTYLE_H

/** Style id type */
typedef quint32 StyleId;

/**
 * Node shape enum
 */
enum NodeShape
{
    NODE_SHAPE_BOX,
    NODE_SHAPE_DEFAULT = NODE_SHAPE_BOX,
    NODE_SHAPE_ROUNDED_BOX,
    NODE_SHAPE_CIRCLE,
    NODE_SHAPE_DIAMOND,
    NODE_SHAPE_ELLIPSE,
    NODE_SHAPES_NUM    
};

/**
 * Convert node shape to string 
 */
static QString
nodeShape2Str( NodeShape shape)
{
    ASSERTD( shape < NODE_SHAPES_NUM);
    switch ( shape)
    {
      case NODE_SHAPE_BOX:
        return QString("box");
      case NODE_SHAPE_ROUNDED_BOX:
        return QString("rounded_box");
      case NODE_SHAPE_CIRCLE:
        return QString("circle");
      case NODE_SHAPE_DIAMOND:
        return QString("diamond");
      case NODE_SHAPE_ELLIPSE:
        return QString("ellipse");
      default:
        ASSERTD( 0);
        return QString(); 
    }
    return QString();
}

/**
 * Convert string to node shape 
 */
static NodeShape
str2NodeShape( const QString &str)
{
    if ( str == "box")
    {
        return NODE_SHAPE_BOX;
    } else if ( str == "rounded_box")
    {
        return NODE_SHAPE_ROUNDED_BOX;
    } else if ( str == "circle")
    {
        return NODE_SHAPE_CIRCLE;
    } else if ( str =="diamond")
    {
        return NODE_SHAPE_DIAMOND;
    } else if ( str == "ellipse")
    {
        return NODE_SHAPE_ELLIPSE;
    } else
    {
        return NODE_SHAPES_NUM;
    }
}

/**
 * Graphic style for nodes and edges
 */ 
class GStyle
{
public:
    /** Constructor */
    inline GStyle();
    /** Copy constructor */
    inline GStyle( const GStyle&);
    /** Copy constructor with name specification */
    inline GStyle( QString nm, const GStyle&);
    /** 
     * Construction from XML description
     * @param Element wih style description
     * @throws GGraphError
     */
    inline GStyle( QDomElement e);
    /** Assignment */
    inline GStyle& operator = ( const GStyle&);
    /** Destructor */
    virtual ~GStyle(){};
    /** Get name of style */
    inline QString name() const;
    /** Get pen */
    inline QPen pen() const;
    /** Get brush */
    inline QBrush brush() const;
    /** Set default/changed state*/
    inline void setState( bool default_state = false);
    /** Set name of style */
    inline void setName( QString &str);
    /** Set pen */
    inline void setPen( QPen &pn);
    /** Set brush */
    inline void setBrush( QBrush &br);
    /** Set pen color */
    inline void setPenColor( QColor &color);
    /** Set pen style */
    inline void setPenStyle( Qt::PenStyle st);
    /** Set pen width */
    inline void setPenWidth( qreal width);
    /** Set brush color */
    inline void setBrushColor( QColor &color);
    /** Check if style uses default pen */
    inline bool isDefault() const;
    /** Get node shape */
    inline NodeShape shape() const;
    /** Set node shape */
    inline void setShape( NodeShape new_shape);
    /** Saving to element */
    inline void writeElement( QDomElement e, bool save_name = true);
    /** Increase items num */
    inline void incNumItems();
    /** Decrease items num */
    inline void decNumItems();
    /** Get number of affected items */
    inline GraphNum numItems();
private:
    QString name_priv;
    QPen pen_priv;
    QBrush brush_priv;
    bool is_default;
    GraphNum num_items;
    NodeShape shape_priv;
    //StyleId id;
};

/** Constructor */
inline GStyle::GStyle(): 
    is_default( true), num_items( 0), shape_priv(NODE_SHAPE_DEFAULT)
{}

/** Copy constructor */
inline GStyle::GStyle( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    brush_priv = st.brush_priv;
    is_default = st.is_default;
    shape_priv = st.shape_priv;
    num_items = 0;
}
/** Copy constructor with name specification */
inline 
GStyle::GStyle( QString nm, const GStyle& st)
{
    num_items = 0;
    name_priv = nm;
    pen_priv = st.pen_priv;
    brush_priv = st.brush_priv;
    is_default = st.is_default;
    shape_priv = st.shape_priv;
}

/** Construction from XML description */
inline 
GStyle::GStyle( QDomElement e ): 
    is_default( true),
    shape_priv(NODE_SHAPE_DEFAULT)
{
    num_items = 0;
    ASSERTD( !e.isNull());
    
    QString error_msg = QString("in line %1: ").arg( e.lineNumber());
        
    /** A style must be named in XML */
    if( e.tagName() == QString( "style") && !e.hasAttribute( "name"))
    {
        throw GGraphError( error_msg.append("style without a name"));
    } else
    {
       name_priv = e.attribute( "name");    
    }
    /** Parse pen color */
    if ( e.hasAttribute( "pen_color"))
    {
        is_default = false;
        QColor color( e.attribute( "pen_color"));
        
        if ( color.isValid())
        {
            pen_priv.setColor( color);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen color"));
        }
    }
    /** Parse pen style */
    if ( e.hasAttribute( "pen_style"))
    {
        is_default = false;
        
        QString stl = e.attribute( "pen_style");
        
        if ( stl == "no_pen")
        {
            pen_priv.setStyle( Qt::NoPen);
        } else if ( stl == "solid")
        {
            pen_priv.setStyle( Qt::SolidLine);
        } else if ( stl == "dash")
        {
            pen_priv.setStyle( Qt::DashLine);
        } else if ( stl == "dot")
        {
            pen_priv.setStyle( Qt::DotLine);
        } else if ( stl == "dash_dot")
        {
            pen_priv.setStyle( Qt::DashDotLine);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen style"));
        }
    }
    /** Parse pen width */
    if ( e.hasAttribute( "pen_width"))
    {
        is_default = false;
        bool ok;
        qreal width = e.attribute( "pen_width").toDouble( &ok);
        
        if ( ok)
        {
            pen_priv.setWidthF( width);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen width"));
        }
    } else
    {
        pen_priv.setWidth( 1);
    }
    /** Parse fill color */
    if ( e.hasAttribute( "fill"))
    {
        is_default = false;
        QString fill = e.attribute( "fill");
        QColor color( fill);
        
        if ( fill == "no_fill")
        {
            brush_priv.setStyle( Qt::NoBrush);
        } else if ( color.isValid())
        {
            brush_priv.setStyle( Qt::SolidPattern);
            brush_priv.setColor( color);
        } else
        {
            throw GGraphError( error_msg.append("invalid fill color"));
        }
    } else
    {
        brush_priv.setStyle( Qt::NoBrush);
    }
    if ( e.hasAttribute("shape"))
    {
        NodeShape shp =  str2NodeShape( e.attribute("shape"));
        if ( shp != NODE_SHAPES_NUM)
        {
            is_default = false;        
            setShape( shp);
        } else
        {
            throw GGraphError( error_msg.append("invalid shape"));
        }
    }
}

/** Saving to element */
inline void 
GStyle::writeElement( QDomElement e, bool save_name)
{
    if ( is_default)
        return;
    
    if ( save_name)
        e.setAttribute( "name", name_priv);

    /** Save pen color */
    e.setAttribute( "pen_color", pen_priv.color().name());

    /** Save pen style */
    Qt::PenStyle pen_style = pen_priv.style();

    switch ( pen_style)
    {
        case Qt::NoPen:
            e.setAttribute( "pen_style","no_pen");
            break;
        case Qt::SolidLine:
            e.setAttribute( "pen_style","solid");
            break;
        case Qt::DashLine:
            e.setAttribute( "pen_style","dash");
            break;
        case Qt::DotLine:           
            e.setAttribute( "pen_style","dot");
            break;
        case Qt::DashDotLine:
            e.setAttribute( "pen_style","dash_dot");
            break;
        default:
            ASSERTD( 0);
            e.setAttribute( "pen_style","solid");
            break;
    }
    /** Save pen width */
    e.setAttribute( "pen_width", pen_priv.widthF());
    
    /** Save fill color */
    if ( !( Qt::NoBrush == brush_priv.style()))
    {
        e.setAttribute( "fill", brush_priv.color().name());
    }
    if ( NODE_SHAPE_DEFAULT != shape())
    {
        e.setAttribute("shape", nodeShape2Str( shape()));
    }
}

/** Assignment */
inline GStyle&
GStyle::operator = ( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    is_default = st.is_default;
    brush_priv = st.brush_priv;
    shape_priv = st.shape_priv;
    num_items = 0;
    return *this;
}

/** Get name of style */
inline QString GStyle::name() const
{
    return name_priv;
}
/** Get pen */
inline QPen GStyle::pen() const
{
    return pen_priv;
}
/** Get brush */
inline QBrush GStyle::brush() const
{
    return brush_priv;
}
/** Check if style uses default pen */
inline bool GStyle::isDefault() const
{
    return is_default;
}

/** Increase items num */
inline void GStyle::incNumItems()
{
    num_items++;
}
/** Decrease items num */
inline void GStyle::decNumItems()
{
    ASSERTD( num_items > 0);
    num_items--;
}
/** Get number of affected items */
inline GraphNum GStyle::numItems()
{
    return num_items;
}

/** Set name of style */
inline void GStyle::setName( QString &str)
{
    name_priv = str;
}
/** Set pen */
inline void GStyle::setPen( QPen &pn)
{
    pen_priv = pn;
}
/** Set brush */
inline void GStyle::setBrush( QBrush &br)
{
    brush_priv = br;
}
/** Set pen color */
inline void GStyle::setPenColor( QColor &color)
{
    pen_priv.setColor( color);
}
/** Set pen style */
inline void GStyle::setPenStyle( Qt::PenStyle st)
{
    pen_priv.setStyle( st);
}
/** Set pen width */
inline void GStyle::setPenWidth( qreal width)
{
    pen_priv.setWidthF( width);
}
/** Set brush color */
inline void GStyle::setBrushColor( QColor &color)
{
    brush_priv.setColor( color);
}
/** Set default/changed state*/
inline void GStyle::setState( bool default_state)
{
    is_default = default_state;
}

/** Get node shape */
inline NodeShape GStyle::shape() const
{
    return shape_priv;
}
/** Set node shape */
inline void GStyle::setShape( NodeShape new_shape)
{
    shape_priv = new_shape;
}
#endif /* GSTYLE_H */