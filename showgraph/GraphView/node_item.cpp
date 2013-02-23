/**
 * @file: node_item.cpp 
 * Drawable node implementation
 */
/* 
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gview_impl.h"

//#define DRAW_AXIS

/** Constant for adjusting item's border rectangle */
const qreal box_adjust = 5;
        

/** We can't create nodes separately, do it through newNode method of graph */
GNode::GNode( GGraph *graph_p, int _id):
    AuxNode( ( AuxGraph *)graph_p, _id),
	_doc( NULL),
	ir_id( GRAPH_MAX_NODE_NUM),
	text_shown( false),
    _style( NULL)
{
    item_p = new NodeItem( this);
    graph()->view()->scene()->addItem( item_p);
	setIRId( id());
    graph()->invalidateRanking();
    if ( graph()->view()->isContext())
    {
        item()->hide();
        setForPlacement( false);
        setPriority( 0);
    }
}

/** Contructor of node with specified position */
GNode::GNode( GGraph *graph_p, int _id, QPointF _pos):
    AuxNode( ( AuxGraph *)graph_p, _id),
	_doc( NULL),
	ir_id( GRAPH_MAX_NODE_NUM),
	text_shown( false),
    _style( NULL)
{
    item_p = new NodeItem( this);
    item_p->setPos( _pos);
	graph()->view()->scene()->addItem( item_p);
	setIRId( id());
    graph()->invalidateRanking();
    if ( graph()->view()->isContext())
    {
        item()->hide();
        setForPlacement( false);
        setPriority( 0);
    }
}

/**
 * Destructor for node - removes edge controls on incidient edges and disconnects item from scene
 */
GNode::~GNode()
{
    graph()->invalidateRanking();
    if ( ( isEdgeControl() || isEdgeLabel())
         && isNotNullP( firstPred()) 
         && isNotNullP( firstSucc())
         && isNotNullP( firstPred()->pred())
         && isNotNullP( firstSucc()->succ()))
    {
        GRAPH_ASSERTD( areEqP( firstPred()->style(), firstSucc()->style()),
                       "Different styles on the same edge");
        GEdge *e = graph()->newEdge( firstPred()->pred(), firstSucc()->succ());
        e->setStyle( firstPred()->style());
    } else if ( isSimple())
    {
        QList< GNode *> nodes;
        GEdge* edge;
		Marker m = graph()->newMarker();
        for ( edge = firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->item()->adjust();
            GNode* succ = edge->succ();

            while ( succ->isEdgeControl() || succ->isEdgeLabel())
            {
                assert( isNotNullP( succ->firstSucc()));
                if ( succ->mark( m))
				{
					nodes << succ;
				}
                succ = succ->firstSucc()->succ();
            }
        }
        for ( edge = firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            if ( edge->isSelf()) // We've already processed this one in previous loop
				continue;

			edge->item()->adjust();
            GNode* pred = edge->pred();

            while ( pred->isEdgeControl() || pred->isEdgeLabel())
            {
                assert( isNotNullP( pred->firstPred()));
                if ( pred->mark( m))
				{
					nodes << pred;
				}
                pred = pred->firstPred()->pred();
            }
        }
        
        foreach ( GNode *n, nodes)
        {
            graph()->deleteNode( n);
        }
		graph()->freeMarker( m);
    }
    if ( isNodeInFocus())
        graph()->setNodeInFocus( NULL);
    graph()->view()->viewHistory()->eraseNode( this);
    item()->remove();
    graph()->view()->deleteLaterNodeItem( item());
    delete _doc;
            
    if ( isNotNullP( _style))
        _style->decNumItems();
}
/**
 * Get the pointer to graph
 */
GGraph* GNode::graph() const
{
    return static_cast< GGraph *>( AuxNode::graph());
}

/**
 * Update DOM tree element
 */
void
GNode::updateElement()
{
    AuxNode::updateElement();// Base class method call
    QDomElement e = elem();
    e.setAttribute( "x", item()->x());
    e.setAttribute( "y", item()->y());
    e.setAttribute( "label", item()->toPlainText());
    if ( isSimple())
    {  
       // e.setAttribute( "type", "simple");
    } else if ( isEdgeControl())
    {
        e.setAttribute( "type", "edge_control");
    } else if ( isEdgeLabel())
    {
        e.setAttribute( "type", "edge_label");
    }
    /** Save style that describes this node only along with node */
    if ( isNotNullP( style())) 
    {     
        if ( 1 == style()->numItems())
        {
            e.removeAttribute("style");
            style()->writeElement( e, false);
        } else
        {
            e.setAttribute("style", style()->name());
        }
    }
}

/**
 * read properties from DOM tree element
 */
void
GNode::readFromElement( QDomElement e)
{
    assert( !e.isNull());
    assert( e.tagName() == QString( "node"));
    
    if ( e.hasAttribute( "x") && e.hasAttribute( "y"))
    {
        qreal x = e.attribute( "x").toDouble();
        qreal y = e.attribute( "y").toDouble();
        item()->setPos( x, y);
    }
    if ( e.hasAttribute( "label"))
    {
        QString str = e.attribute( "label");
        item()->setPlainText( str);
        QRegExp rx("(\\d+)");
        if ( rx.indexIn( str) != -1)
        {
            setIRId( rx.cap( 1).toInt());
        }
    }
    if ( e.hasAttribute("type"))
    {
        QString type_str = e.attribute( "type");
        if ( type_str == QString( "edge_control"))
        {
            setTypeEdgeControl();
        } else if (type_str == QString( "edge_label"))
        {
            setTypeEdgeLabel();
        } 
    }

    AuxNode::readFromElement( e); // Base class method
}
bool GNode::isNodeInFocus() const
{
    return areEqP( this, graph()->nodeInFocus());
}

/*********************** NodeItem implementation ***************************************/


/** Initialization */
void 
NodeItem::SetInitFlags()
{
    setFlag( ItemIsMovable);
    setFlag( ItemIsFocusable);
// needed for Qt 4.6 and higher
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
    setFlag( ItemSendsGeometryChanges);
#endif
    setCacheMode( DeviceCoordinateCache);
    setZValue(2);
    QGraphicsItem::setCursor( Qt::ArrowCursor);
    /**
     * Sets text to create text control. 
     * FIXME: Should find a way to prevent this for edgeControl nodes, since text
     *        contol is not needed for them and this procedure is quite expensive
     */
    setPlainText( "");
}

/** Path of box type that given inner rectangle */
inline QRectF boxRect( QRectF rect)
{
    return rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust);
}
/** Bounding rect that outlines the shape of box type for given inner rectangle */
inline QPainterPath boxPath( QRectF rect)
{
    QPainterPath path;
    path.addRect( rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust));
    return path;
}

/** Path of rounded box type that given inner rectangle */
inline QRectF rboxRect( QRectF rect)
{
    return rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust);
}
/** Bounding rect that outlines the shape of rounded box type for given inner rectangle */
inline QPainterPath rboxPath( QRectF rect)
{
    QPainterPath path;
    path.addRoundedRect( rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust), 2*box_adjust, 2*box_adjust);
    return path;
}

/** Path of circle type that given inner rectangle */
inline QRectF circleRect( QRectF rect)
{
    QRectF recta = rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust);
    qreal radius = sqrt( (recta.width() * recta.width()) + (recta.height() * recta.height())) /2;
    return QRectF( -radius + rect.width() /2, -radius + rect.height() /2, 2*radius, 2*radius);
}
/** Bounding rect that outlines the shape of circle type for given inner rectangle */
inline QPainterPath circlePath( QRectF rect)
{
    QPainterPath path;
    path.addEllipse( circleRect( rect));
    return path;
}

/** Path of diamond type that given inner rectangle */
inline QRectF diamondRect( QRectF rect)
{
    return rect.adjusted( -rect.width() /2, -rect.height() /2, rect.width() /2, rect.height() /2);
}
/** Bounding rect that outlines the shape of diamond type for given inner rectangle */
inline QPainterPath diamondPath( QRectF rect)
{
    QRectF drect = diamondRect( rect);
    QPainterPath path( drect.center() - QPointF(drect.width() / 2, 0));
    path.lineTo( drect.center() - QPointF(0, drect.height() / 2));
    path.lineTo( drect.center() + QPointF(drect.width() / 2, 0));
    path.lineTo( drect.center() + QPointF(0, drect.height() / 2));
    path.closeSubpath();

    return path;
}

/** Path of ellipse type that given inner rectangle */
inline QRectF ellipseRect( QRectF rect)
{
    return diamondRect( rect);
}
/** Bounding rect that outlines the shape of ellipse type for given inner rectangle */
inline QPainterPath ellipsePath( QRectF rect)
{
    QPainterPath path;
    path.addEllipse( ellipseRect( rect));
    return path;
}

void NodeItem::shapeChanged()
{
    prepareGeometryChange();
}
/**
 * Implementation of shape calculation
 */
inline QPainterPath
shape2Path( NodeShape shape, QRectF rect)
{
    ASSERTD( shape < NODE_SHAPES_NUM);
    switch ( shape)
    {
      case NODE_SHAPE_BOX:
        return boxPath( rect);
      case NODE_SHAPE_ROUNDED_BOX:
        return rboxPath( rect);
      case NODE_SHAPE_CIRCLE:
        return circlePath( rect);
      case NODE_SHAPE_DIAMOND:
        return diamondPath( rect);
      case NODE_SHAPE_ELLIPSE:
        return ellipsePath( rect);
      default:
        ASSERTD( 0);
        QPainterPath path;
        path.addRect( rect);
        return path;
    }
        QPainterPath path;
    path.addRect( rect);
    return path;
}

/**
 * Implementation of shape calculation
 */
inline QRectF
shape2Rect( NodeShape shape, QRectF rect)
{
    ASSERTD( shape < NODE_SHAPES_NUM);
    switch ( shape)
    {
      case NODE_SHAPE_BOX:
        return boxRect( rect);
      case NODE_SHAPE_ROUNDED_BOX:
        return rboxRect( rect);
      case NODE_SHAPE_CIRCLE:
        return circleRect( rect);
      case NODE_SHAPE_DIAMOND:
        return diamondRect( rect);
      case NODE_SHAPE_ELLIPSE:
        return ellipseRect( rect);
      default:
        ASSERTD( 0);
        return rect; 
    }
    return rect;
}

/**
 * Rectangle that marks border of node
 */
QRectF 
NodeItem::borderRect() const
{
    if ( isNullP( node_p))
        return QRectF();

    if ( node()->isEdgeControl())
    {
        qreal adjust = 2;
        return QRectF( -EdgeControlSize - adjust, -EdgeControlSize - adjust,
              2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
    } else if ( node()->isEdgeLabel())
    {
        return QGraphicsTextItem::boundingRect()
            .adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust);    
    } else
    {
        QRectF rect =  QGraphicsTextItem::boundingRect();
        if ( isNotNullP( node()->style()))
        {
            return shape2Rect( node()->style()->shape(), rect);
        } else
        {
            return rect.adjusted( -box_adjust, -box_adjust, box_adjust, box_adjust);
        }
    }
}

/**
 * Overload of QGraphicsItem::bounding rectangle
 */
QRectF 
NodeItem::boundingRect() const
{
    qreal adjust = 2;
    return borderRect().adjusted( -adjust, -adjust, adjust, adjust);
}

/**
 * Shape of NodeItem: circle for EdgeControl and rectangle for simple
 */
QPainterPath 
NodeItem::shape() const
{
    if ( isNullP( node_p))
        return QPainterPath();

    if ( node()->isEdgeControl())
    {
        QPainterPath path;
        path.addEllipse( -EdgeControlSize, -EdgeControlSize, 2*EdgeControlSize, 2*EdgeControlSize);
        return path; 
    } else
    {
        QRectF rect =  QGraphicsTextItem::boundingRect();
        if ( isNotNullP( node()->style()))
        {
            return shape2Path( node()->style()->shape(), rect);
        } else
        {
            QPainterPath path;
            path.addRect( borderRect());
            return path;
        }
    }
}

bool NodeItem::contains(const QPointF &point) const
{
    return shape().contains(point);
}

/**
 * Painting procedure for NodeItem
 */
void 
NodeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( isNullP( node_p))
        return;
    
    if ( node()->graph()->view()->isContext())
        painter->setOpacity( opacityLevel());

    if ( node()->isSimple() || node()->isEdgeLabel())
    {
#ifdef DRAW_AXIS
        QPen axis_pen( "red");
        painter->setPen( axis_pen);
        painter->drawRect( borderRect());
        painter->drawLine( QPoint( -boundingRect().width(), 0), QPoint( boundingRect().width(),0));
        painter->drawLine( QPoint( 0, -boundingRect().height()), QPoint( 0, boundingRect().height()));
#endif
        QPen pen( option->palette.foreground().color(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        if ( option->levelOfDetail < 0.1)
        {
            painter->fillRect( borderRect(), option->palette.highlight().color());
        }
        qreal adjust = 3;
        
        if ( isNotNullP( node()->style()))
        {
            pen = node()->style()->pen();
            painter->setBrush( node()->style()->brush());
        }
        if ( bold_border )// ( option->state & QStyle::State_Sunken))
        {
            pen.setWidthF( pen.widthF() + 1);
        } 
        if ( alternate_background)
        {
            painter->setBrush( option->palette.highlight().color());
        }
        
        painter->setPen( pen);

        if ( node()->isSimple())
        {
            QRectF rect =  QGraphicsTextItem::boundingRect();
            if ( isNotNullP( node()->style()))
            {
                painter->drawPath( shape2Path( node()->style()->shape(), rect));
            } else
            {
                painter->drawRect( borderRect());
            }
        }
        if ( option->levelOfDetail >= 0.2)
        {
            if ( painter->isActive())     
                QGraphicsTextItem::paint( painter, option, widget);
        }
    } else if ( node()->isEdgeControl())
    {
        if ( option->levelOfDetail < 0.2)
            return;
        if ( node()->firstPred()->item()->isSelected()
             || node()->firstSucc()->item()->isSelected())
        {
            if ( bold_border && ( option->state & QStyle::State_Sunken)) 
            {
                painter->setBrush( option->palette.highlight().color());
                painter->setPen( QPen(option->palette.foreground().color(), 0));
            } else
            {
                painter->setBrush( option->palette.highlight().color());
                painter->setPen( QPen(option->palette.foreground().color(), 0));
            }
            painter->drawEllipse( -EdgeControlSize, -EdgeControlSize,
                                  2*EdgeControlSize, 2*EdgeControlSize);
        }
    }
    painter->setOpacity( 1);
}

/**
 * Right button press starts edge drawing process 
 */
void NodeItem::mousePressEvent( QGraphicsSceneMouseEvent *event)
{
    bold_border = true;
	if ( event->button() & Qt::RightButton && !node()->isEdgeControl())
    {
        if ( node()->graph()->view()->isEditable())
        {
            node()->graph()->view()->SetCreateEdge( true);
            node()->graph()->view()->SetTmpSrc( node());
            node()->graph()->view()->showHelper();
        }
    } else if ( node()->isEdgeControl() || node()->isEdgeLabel())
    {
        node()->firstPred()->item()->setSelected( true);
        node()->firstSucc()->item()->setSelected( true);
    }
    QGraphicsTextItem::mousePressEvent(event);
    update();
}

/**
 * On mouse release we do nothing - graph will handle it for us
 */
void NodeItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
{
    bold_border = false;
    bool call_baseclass = true;

	/** Select this node */
	node()->graph()->emptySelection();
    if ( !node()->isNodeInFocus())
    {
        node()->graph()->view()->viewHistory()->focusEvent( node());
        node()->graph()->setNodeInFocus( node());
    }
	node()->graph()->selectNode( this->node());
	/** Show context menu */
	if ( node()->graph()->view()->isShowContextMenus()
        && ( event->button() & Qt::RightButton) )
    {
	    QMenu *menu = node()->graph()->view()->createMenuForNode( node());
        menu->exec( event->screenPos());
        call_baseclass = false;
        delete menu;
    } else if ( event->button() & Qt::LeftButton 
                && !( node()->isEdgeControl() || node()->isEdgeLabel()))
    {
        if (  node()->graph()->view()->isContext())
        {
            node()->graph()->view()->findContext();
            //node()->graph()->view()->showNodeText( node());
        } else
        {
            //node()->graph()->view()->showNodeText( node());
        }
	}
	if ( call_baseclass)
        QGraphicsTextItem::mouseReleaseEvent( event);
	update();
    setFlag( ItemIsMovable, true);
}

/**
 * Double click enables text edition for simple nodes
 */
void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setFlag( ItemIsMovable, false);
    if ( node()->graph()->view()->isEditable())
    {
        if ( event->button() & Qt::LeftButton && !node()->isEdgeControl())
        {
            if ( textInteractionFlags() == Qt::NoTextInteraction)
            {
                setTextInteractionFlags(Qt::TextEditorInteraction);
                setFocus( Qt::MouseFocusReason);
            }
			    //QGraphicsTextItem::mousePressEvent(event);
        }
    } else
    {
        node()->graph()->view()->showNodeText( node());
    }
}

void NodeItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
	QGraphicsTextItem::focusOutEvent(event);
    setFlag( ItemIsFocusable, true);
}

/**
 * We should adjust edges when entering the text as the size of item changes
 */
void NodeItem::keyPressEvent(QKeyEvent *event)
{
    
    prepareGeometryChange();
    if ( textInteractionFlags() == Qt::TextEditorInteraction)
    {
        GEdge *edge = NULL;
        QGraphicsTextItem::keyPressEvent(event);
        for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->item()->adjust();
        }
        for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            edge->item()->adjust();
        }
    } else
    {
        int key = event->key();
        GEdge *edge = NULL;
        NavSector sector = UNDEF_SECTOR;
        if ( node()->isNodeInFocus())
        {
            switch( key)
            {
                case Qt::Key_Up:
                    sector = TOP_SECTOR;
                    break;
                case Qt::Key_Down:
                    sector = BOTTOM_SECTOR;
                    break;
                case Qt::Key_Left:
                    sector = LEFT_SECTOR;
                    break;
                case Qt::Key_Right:
                    sector = RIGHT_SECTOR;
                    break;
                default:
                    sector = UNDEF_SECTOR;
                    break;
            }
            
            node()->graph()->setNodeInFocus( node(), sector);
            edge = NodeNav( node(), sector).firstEdgeInSector();
                    
            if ( isNotNullP( edge))
            {
                // Get focus on edge
                scene()->clearFocus();
                scene()->clearSelection();
                edge->item()->setFocus();
                edge->item()->setSelected( true);
            }
        }
    }
    update();
}

/** 
 * Perform animation step
 * Advance node's coordinates and opacity towards goal values of these parameters
 * Return true if node have advanced somehow. False if node hasn't change
 */
bool NodeItem::advance()
{
    QPointF target( node()->modelX(), node()->modelY());
    QLineF line( pos(), target); 
    qreal dist = line.length();
    bool changed = false;

    if ( !isVisible())
        return false;

    qreal target_opacity = ((qreal)node()->priority())/ MAX_PRIORITY;
    
    if ( target_opacity > opacityLevel() + OPACITY_STEP)
    {
        setOpacityLevel( opacityLevel() + OPACITY_STEP);
        changed = true;
    } else if ( target_opacity < opacityLevel() - OPACITY_STEP)
    {
        setOpacityLevel( opacityLevel() - OPACITY_STEP);
        changed = true;
    } else
    {
        setOpacityLevel( target_opacity);
        if ( !target_opacity)
        {
            setVisible( false);
            changed = true;
        }
    }
    if ( changed)
    {
        adjustAssociates();
        updateAssociates();
        update();
    }

    if ( dist > NODE_SPEED)
    {
        QPointF displacement( NODE_SPEED * line.dx() / dist, NODE_SPEED * line.dy() / dist);
        setPos( pos() + displacement);   
        changed = true;
    }
    return changed;
}

void NodeItem::adjustAssociates()
{
    GEdge *edge = NULL;

    for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->item()->adjust();
        GNode* succ = edge->succ();

        if ( succ->isEdgeControl() || succ->isEdgeLabel())
        {
            assert( isNotNullP( succ->firstSucc()));
            succ->firstSucc()->item()->adjust();
        }
    }
    for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->item()->adjust();
        GNode* pred = edge->pred();

        if ( pred->isEdgeControl() || pred->isEdgeLabel())
        {
            assert( isNotNullP( pred->firstPred()));
            pred->firstPred()->item()->adjust();
        }
    }
    prepareGeometryChange();
}

void NodeItem::updateAssociates()
{
    GEdge *edge = NULL;

    for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->item()->adjust();
        GNode* succ = edge->succ();

        if ( succ->isEdgeControl() || succ->isEdgeLabel())
        {
            assert( isNotNullP( succ->firstSucc()));
            succ->firstSucc()->item()->update();
        }
    }
    for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->item()->adjust();
        GNode* pred = edge->pred();

        if ( pred->isEdgeControl() || pred->isEdgeLabel())
        {
            assert( isNotNullP( pred->firstPred()));
            pred->firstPred()->item()->update();
        }
    }
}
/**
 * Adjust edges when node changes
 */
QVariant NodeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    GEdge *edge = NULL;

    if ( change != QGraphicsItem::ItemSceneChange 
         || change != QGraphicsItem::ItemSceneHasChanged)
    {
        adjustAssociates();
    }
    return QGraphicsTextItem::itemChange(change, value);
}


