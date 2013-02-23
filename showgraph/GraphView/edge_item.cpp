/**
 * @file: edge_item.cpp 
 * Drawable edge implementation.
 * GUI for ShowGraph tool.
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gview_impl.h"
#include <QVector>

//#define SHOW_CONTROL_POINTS
//#define SHOW_BACKEDGES

GEdge::GEdge( GGraph *graph_p, int _id, GNode *_pred, GNode* _succ):
    AuxEdge( (AuxGraph *)graph_p, _id, (AuxNode *)_pred, (AuxNode *)_succ), _style( NULL)
{
    item_p = new EdgeItem( this);
    graph()->view()->scene()->addItem( item_p);
    item_p->adjust();
    graph()->invalidateRanking();
}
   
GEdge::~GEdge()
{
    graph()->invalidateRanking();
    item()->remove();
    graph()->view()->deleteLaterEdgeItem( item());
    if ( isNotNullP( _style))
        _style->decNumItems();
}

GNode * 
GEdge::node( GraphDir dir) const 
{
    return static_cast< GNode *>(AuxEdge::node( dir));
}

GNode * 
GEdge::realNode( GraphDir dir) const 
{
    return static_cast< GNode *>(AuxEdge::realNode( dir));
}

GGraph *
GEdge::graph() const
{
    return static_cast< GGraph *>( AuxEdge::graph());
}

/**
 * Update DOM tree element
 */
void
GEdge::updateElement()
{
#if 0    
    QDomElement e = elem();
    int i = 0;
    QDomElement new_e = graph()->createElement( "edge");
    QDomNode e2 = graph()->documentElement().removeChild( e);
    assert( !e2.isNull());
    graph()->documentElement().appendChild( new_e);
    setElement( new_e);
    e = new_e;
#endif
    /* Base class method call to print generic edge properties */
    AuxEdge::updateElement();
    QDomElement e = elem();
    /** Save style that describes this edge only along with edge */
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

/** Make all styles of edges connected with edge control or label the same */
void
GEdge::adjustStyles()
{
    GNode* succ_n = succ();

    while ( succ_n->isEdgeControl() || succ_n->isEdgeLabel())
    {
        assert( isNotNullP( succ_n->firstSucc()));
        succ_n->firstSucc()->setStyle( style());
        succ_n = succ_n->firstSucc()->succ();
    }

    GNode* pred_n = pred();

    while ( pred_n->isEdgeControl() || pred_n->isEdgeLabel())
    {
        assert( isNotNullP( pred_n->firstPred()));
        pred_n->firstPred()->setStyle( style());
        pred_n = pred_n->firstPred()->pred();
    }
}
/**
 * read properties from DOM tree element
 */
void
GEdge::readFromElement( QDomElement e)
{
    AuxEdge::readFromElement( e); // Base class method
    item()->adjust();
}
/**
 * Insert node of label type
 */
GNode *
GEdge::insertLabelNode( QPointF pos)
{
    GNode *new_node = static_cast<GNode *>( insertNode());
    new_node->setTypeEdgeLabel();
    new_node->item()->setPlainText( "Label");
    new_node->item()->setPos( pos + QPointF( new_node->item()->borderRect().width()/2, 0));
    
    return new_node;
}

/**
 * Insert node with style propagation
 */
AuxNode *
GEdge::insertNode()
{
    GStyle *st = style();
    GNode *new_node = static_cast<GNode *>( AuxEdge::insertNode());
    //new_node->firstPred()->setStyle( st);
    new_node->firstSucc()->setStyle( st);
    return new_node;
}

/**
 * EdgeItem implementaion
 */
/** Constructor */
EdgeItem::EdgeItem( GEdge *e_p): edge_p( e_p)
{
    curr_mode = ModeShow;
    QGraphicsItem::setCursor( Qt::ArrowCursor);
    setFlag( ItemIsSelectable);
    setFlag( ItemIsFocusable);
    //setCacheMode( DeviceCoordinateCache);
    setZValue(1);
}

QVariant
EdgeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedChange)
    {
        pred()->item()->update();
        succ()->item()->update();
    }
    return QGraphicsItem::itemChange( change, value);
}    

/** Convenience routine for self edge path */
QPainterPath EdgeItem::selfEdgePath() const
{
    assertd( edge()->isSelf());
    QPainterPath path( srcP);
    QPointF center = mapFromItem( pred()->item(), pred()->item()->boundingRect().center());
    QRectF r = pred()->item()->borderRect();
    
    path.cubicTo(  center + QPointF( 3 * r.width()/8, r.height()/2 + SE_VERT_MARGIN),
                   btmRight, cp1);
    path.cubicTo(  cp1 + QPointF( 0, -r.height()/2-SE_VERT_MARGIN),
                   topLeft, dstP);
    return path;
}

void
EdgeItem::adjust()
{
    prepareGeometryChange();
    if ( edge()->pred()->item()->isVisible()
         && edge()->succ()->item()->isVisible())
    {
        setVisible( true);
    } else
    {
        setVisible( false);
        return;
    }
    if ( edge()->isSelf())
    {
        QPointF center = mapFromItem( pred()->item(), pred()->item()->borderRect().center());
        QRectF r = pred()->item()->borderRect();
        srcP = center + QPointF( 3 * r.width()/8, r.height() /2);
        dstP = center + QPointF( 3 * r.width()/8, -r.height() /2);
        topLeft = center + QPointF( 3 * r.width()/8, -r.height()/2 - SE_VERT_MARGIN);
        cp1 = center + QPointF( (r.width() / 2) + SE_HOR_MARGIN, 0);
        btmRight = cp1 + QPointF( 0, r.height()/2 + SE_VERT_MARGIN);
        
        update();
        return;
    }
    srcP = mapFromItem( pred()->item(), pred()->item()->boundingRect().center());
    dstP = mapFromItem( succ()->item(), succ()->item()->boundingRect().center());
    topLeft = srcP;
    btmRight = dstP;
    QPointF srcCP = srcP;
    QPointF dstCP = dstP;

    if ( pred()->isEdgeLabel())
    {
        srcP = mapFromItem( pred()->item(),
                            pred()->item()->borderRect().left(),
                            pred()->item()->borderRect().center().y());
        qreal w = pred()->item()->borderRect().width();
        //srcP += QPointF( -w, 0);
        srcCP = srcP;
    } 
    if ( succ()->isEdgeLabel())
    {
        dstP = mapFromItem( succ()->item(),
                            succ()->item()->borderRect().left(),
                            succ()->item()->borderRect().center().y());
        dstCP = dstP;
    } 

    if ( pred()->isSimple())
    {
        QLineF line( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( pred()->item(), pred()->item()->shape().toFillPolygon());
        QPointF p1 = endPolygon.first();
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i);
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                 polyLine.intersect( line, &srcP);
            if (intersectType == QLineF::BoundedIntersection)
                 break;
            p1 = p2;
        }
    }
    if ( succ()->isSimple())
    {
        QLineF line2( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( succ()->item(), succ()->item()->shape().toFillPolygon());
        QPointF p1 = endPolygon.first();;
        QPointF p2;
        QLineF polyLine;
        
        for ( int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i);
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                 polyLine.intersect( line2, &dstP);
            if ( intersectType == QLineF::BoundedIntersection)
                 break;
            p1 = p2;
        }
    }
    topLeft.setX( min< qreal>( srcP.x(), dstP.x()));
    topLeft.setY( min< qreal>( srcP.y(), dstP.y()));
    btmRight.setX( max< qreal>( srcP.x(), dstP.x()));
    btmRight.setY( max< qreal>( srcP.y(), dstP.y()));

    QLineF mainLine = QLineF( srcP, dstP);
        
    if ( mainLine.length() < 1)
        return;

    qreal size = abs< qreal>(( min< qreal>( abs< qreal>( mainLine.dx()),
                                            abs< qreal>( mainLine.dy()))));
    //Stub
    if( size < 2* EdgeControlSize)
    {
        size = 2* EdgeControlSize;
    }
    if( size > 20 * EdgeControlSize)
    {
        size = 20 * EdgeControlSize;
    }
    
    NodeItem *next_pred = NULL;
    NodeItem *next_succ = NULL;

    if ( ( pred()->isEdgeControl() || pred()->isEdgeLabel()) 
         && isNotNullP( pred()->firstPred()))
    {
        next_pred = pred()->firstPred()->pred()->item();
    } 
    if ( ( succ()->isEdgeControl() || succ()->isEdgeLabel()) 
         && isNotNullP( succ()->firstSucc()))
    {
        next_succ = succ()->firstSucc()->succ()->item();
    } 
    /** Place cp1 */
    if ( isNotNullP( next_pred))
    {
        QPointF p1 = mapFromItem( next_pred, next_pred->borderRect().center());
        QLineF line( p1, dstCP);
        QPointF cp1_offset = QPointF( (line.dx() * size)/ line.length(),
                                      (line.dy() * size)/ line.length());
        cp1 = srcP + cp1_offset;
    } else
    {
        QPointF cp1_offset = QPointF( (mainLine.dx() * size)/ mainLine.length(),
                                      (mainLine.dy() * size)/ mainLine.length());
        cp1 = srcP + cp1_offset;
    }
    
    topLeft.setX( min< qreal>( topLeft.x(), cp1.x()));
    topLeft.setY( min< qreal>( topLeft.y(), cp1.y()));
    btmRight.setX( max< qreal>( btmRight.x(), cp1.x()));
    btmRight.setY( max< qreal>( btmRight.y(), cp1.y()));

    /** Place cp2 */
    if ( isNotNullP( next_succ))
    {
        QPointF p2 = mapFromItem( next_succ, next_succ->borderRect().center());
        QLineF line( p2, srcCP);
        QPointF cp2_offset = QPointF( (line.dx() * size)/ line.length(),
                                      (line.dy() * size)/ line.length());
        cp2 = dstP + cp2_offset;
    } else
    {
        QPointF cp2_offset = QPointF( -(mainLine.dx() * size)/ mainLine.length(),
                                      -(mainLine.dy() * size)/ mainLine.length());
        cp2 = dstP + cp2_offset;
    }
    
    topLeft.setX( min< qreal>( topLeft.x(), cp2.x()));
    topLeft.setY( min< qreal>( topLeft.y(), cp2.y()));
    btmRight.setX( max< qreal>( btmRight.x(), cp2.x()));
    btmRight.setY( max< qreal>( btmRight.y(), cp2.y()));

    update();
}

QRectF 
EdgeItem::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = arrowSize;

    return QRectF( topLeft,
                   QSizeF( btmRight.x() - topLeft.x(),
                           btmRight.y() - topLeft.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

QPainterPath 
EdgeItem::shape() const
{
    QPainterPath path( srcP);
    QPainterPathStroker stroker;
        
    if ( srcP == dstP)
        return path;
    
    if ( edge()->isSelf())
    {
        path = selfEdgePath();
    } else
    {
        path.cubicTo( cp1, cp2, dstP);
    }
    if ( isNotNullP( edge()->style()))
    {
        stroker.setWidth( edge()->style()->pen().width() + 1);
    } else
    {
        stroker.setWidth( 2);
    }
    return stroker.createStroke( path); 
}

void 
EdgeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( option->levelOfDetail < 0.1)
        return;

    /** Do not draw edges when adjacent nodes intersect */
    QPolygonF pred_rect = mapFromItem( pred()->item(), pred()->item()->boundingRect());
    QPolygonF succ_rect = mapFromItem( succ()->item(), succ()->item()->boundingRect());
    if ( !succ_rect.intersected( pred_rect).isEmpty())
        return;

    static const qreal spline_detail_level = 0.4;
    static const qreal draw_arrow_detail_level = 0.3;
    QPointF curr_point;
    QLineF line = QLineF();
    curr_point = srcP;
    QPointF nextToDst = srcP;
    qreal opacity = min<qreal>( edge()->pred()->item()->opacityLevel(), 
                              edge()->succ()->item()->opacityLevel());
    line.setP1( nextToDst);
    line.setP2( dstP);
    
    QPainterPath path( srcP);
    QPainterPathStroker stroker;
    stroker.setWidth( 0);

    if ( edge()->isSelf())
    {
        path = selfEdgePath();
    } else if ( option->levelOfDetail >= spline_detail_level)
    {
        path.cubicTo( cp1, cp2, dstP);
    }
    //path = stroker.createStroke(path);
    if ( nextToDst == dstP)
        return;

    //Set opacity
    if ( edge()->graph()->view()->isContext())
        painter->setOpacity( opacity);

    QPen pen( option->palette.foreground().color(),
              1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        
    if ( isNotNullP( edge()->style()))
    {
        pen = edge()->style()->pen();
    }

    // Draw the line itself
    if ( option->levelOfDetail >= spline_detail_level)
    {
        if ( option->state & QStyle::State_Selected)
        {
            pen.setWidthF( pen.widthF() + 1);
        } 
    } else
    {
        pen = QPen( pen.color(),1);
    }

    painter->setPen( pen);
    
    //Draw edge
    if ( edge()->isSelf() || option->levelOfDetail >= spline_detail_level)
    {
        painter->drawPath( path);
    } else
    {
        painter->drawLine( line);
    }
    
    // Draw the arrows if there's enough room and level of detail is appropriate
    if ( option->levelOfDetail >= draw_arrow_detail_level)
    {
        double angle = ::acos(line.dx() / line.length());
        if ( line.dy() >= 0)
            angle = TwoPi - angle;
        
        QPointF destArrowP1;
        QPointF destArrowP2;

       
        /* NOTE:  Qt::black can be replaced by option->palette.foreground().color() */
        if ( isNotNullP( edge()->style()))
        {
            painter->setBrush( edge()->style()->pen().color());
        } else
        {
            painter->setBrush(option->palette.foreground().color());
        }
        if ( edge()->isSelf())
        {
            angle = -2* Pi/3;
        }  
        destArrowP1 = dstP + QPointF( sin(angle - Pi / 3) * arrowSize,
                                      cos(angle - Pi / 3) * arrowSize);
        destArrowP2 = dstP + QPointF( sin(angle - Pi + Pi / 3) * arrowSize,
                                      cos(angle - Pi + Pi / 3) * arrowSize);
        pen.setStyle( Qt::SolidLine);
        painter->setPen( pen);
        if ( succ()->isSimple())
        {
            QPainterPath arrow_path;
            arrow_path.addPolygon( QPolygonF() << dstP << destArrowP1 << destArrowP2 <<  dstP);
            //path = path.united( arrow_path);
            painter->drawPolygon(QPolygonF() << dstP << destArrowP1 << destArrowP2);
        }
    }

    painter->setOpacity( 1);
#ifdef SHOW_CONTROL_POINTS
    /** For illustrative purposes */
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    if ( !edge()->isSelf())
    {
        painter->drawLine( srcP, dstP);
        painter->drawLine( srcP, cp1);
        painter->drawLine( cp2, dstP);
    }   
    painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint( srcP);
    painter->setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint( dstP);
    painter->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint( cp1);
#endif
}
void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        adjust();
    }
    if ( event->button() == Qt::LeftButton && (flags() & ItemIsSelectable))
    {
        bool multiSelect = ( event->modifiers() & Qt::ControlModifier) != 0;
        if ( !multiSelect)
        {
            if ( !isSelected())
            {
                if (scene())
                    scene()->clearSelection();
                setSelected(true);
            }
        }
    } else if ( !(flags() & ItemIsMovable))
    {
//            event->ignore();
    }
}
void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    update();
    /** Select this edge */
    edge()->graph()->emptySelection();
    edge()->graph()->selectEdge( this->edge());
    /** Show context menu */
    if ( ev->button() & Qt::RightButton)
    {
        QMenu *menu = edge()->graph()->view()->createMenuForEdge( edge());
        edge()->graph()->view()->setCurrPos( ev->pos());
        menu->exec( ev->screenPos());
        delete menu;
    }
    QGraphicsItem::mouseReleaseEvent( ev);
}

void EdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
    if( edge()->graph()->view()->isEditable() 
        && ev->button() & Qt::LeftButton)
    {
        if ( edge()->isSelf())
        {
            GNode* new_node1 = static_cast<GNode *>( edge()->insertNode());
            GNode* new_node2 = static_cast<GNode *>( edge()->insertNode());
            new_node1->setTypeEdgeControl();
            new_node2->setTypeEdgeControl();
            new_node1->item()->setPos( ev->pos() + QPointF( 0, -10));
            new_node2->item()->setPos( ev->pos() + QPointF( 0, 10));
        } else
        {
            GNode* new_node = static_cast<GNode *>( edge()->insertNode());
            new_node->setTypeEdgeControl();
            new_node->item()->setPos( ev->pos());
        }
    }
}

void 
EdgeItem::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    GNode *n = NULL;
    GEdge *e = NULL;
    VEdge vedge( edge());
    NavSector sector = edge()->graph()->nodeNavigationSector();
    GNode *curr_node = edge()->graph()->nodeInFocus();
    NodeNav node_nav( curr_node, sector);
    switch( key)
    {
        case Qt::Key_Up:
            if ( isNotNullP( curr_node) 
                 && ( sector == LEFT_SECTOR || sector == RIGHT_SECTOR))
            {
                e = node_nav.edgeInDir( edge(), NAV_DIR_UP);
            } else
            {
                n = vedge.nodeUp();
            }
            break;
        case Qt::Key_Down:
            if ( isNotNullP( curr_node) 
                 && ( sector == LEFT_SECTOR || sector == RIGHT_SECTOR) )
            {
                e = node_nav.edgeInDir( edge(), NAV_DIR_DOWN);
            } else
            {
                n = vedge.nodeDown();
            }
            break;
        case Qt::Key_Left:
            if ( isNotNullP( curr_node) 
                 && ( sector == TOP_SECTOR || sector == BOTTOM_SECTOR) )
            {
                e = node_nav.edgeInDir( edge(), NAV_DIR_LEFT);
            } else
            {
                n = vedge.nodeLeft();
            }
            break;
        case Qt::Key_Right:
            if ( isNotNullP( curr_node) 
                 && ( sector == TOP_SECTOR || sector == BOTTOM_SECTOR) )
            {
                e = node_nav.edgeInDir( edge(), NAV_DIR_RIGHT);
            } else
            {
                n = vedge.nodeRight();
            }
            break;
        default:
            break;
    }
    if ( isNotNullP( n))
    {
        if ( edge()->graph()->view()->isContext())
        {
            edge()->graph()->emptySelection();
            edge()->graph()->selectNode( n);
            edge()->graph()->view()->findContext();
        }
        edge()->graph()->view()->focusOnNode( n, true);
        
        scene()->clearFocus();
        scene()->clearSelection();
        n->item()->setFocus();
        n->item()->setSelected( true);
    } else if ( isNotNullP( e))
    {
        // Get focus on edge
        scene()->clearFocus();
        scene()->clearSelection();
        e->item()->setFocus();
        e->item()->setSelected( true);
    }
    //QGraphicsItem::keyPressEvent( event);
}

