/**
 * @file: edge_helper.cpp 
 * Edge helper class implementation.
 * GUI for ShowGraph tool.
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gview_impl.h"

/**
 * EdgeHelper implementaion
 */
/** Constructor */
EdgeHelper::EdgeHelper(): 
    src_item( NULL),
    dst_item( NULL),
    state( HELPER_STATE_INITIAL)
{
    //setZValue(1);
}

/** Convenience routine for self edge path */
QPainterPath EdgeHelper::selfEdgePath() const
{
    QPainterPath path( srcP);
    QPointF center = mapFromItem( src_item, src_item->boundingRect().center());
    QRectF r = src_item->borderRect();
    
    path.cubicTo(  center + QPointF( 3 * r.width()/8, r.height()/2 + SE_VERT_MARGIN),
                   btmRight, cp1);
    path.cubicTo(  cp1 + QPointF( 0, -r.height()/2-SE_VERT_MARGIN),
                   topLeft, dstP);
    return path;
}

void
EdgeHelper::adjust()
{
    if ( isNullP( src_item) || state == HELPER_STATE_INITIAL)
        return;
    
    prepareGeometryChange();

    bool is_self = ( src_item == dst_item);
    if ( is_self)
    {
        QPointF center = mapFromItem( src_item, src_item->borderRect().center());
        QRectF r = src_item->borderRect();
        srcP = center + QPointF( 3 * r.width()/8, r.height() /2);
        dstP = center + QPointF( 3 * r.width()/8, -r.height() /2);
        topLeft = center + QPointF( 3 * r.width()/8, -r.height()/2 - SE_VERT_MARGIN);
        cp1 = center + QPointF( (r.width() / 2) + SE_HOR_MARGIN, 0);
        btmRight = cp1 + QPointF( 0, r.height()/2 + SE_VERT_MARGIN);
        
        prepareGeometryChange();
        return;
    }
    
    srcP = mapFromItem( src_item, src_item->boundingRect().center());
    if ( isNotNullP( dst_item))
        dstP = mapFromItem( dst_item, dst_item->boundingRect().center());
    
    topLeft = srcP;
    btmRight = dstP;
    QPointF srcCP = srcP;
    QPointF dstCP = dstP;

    /** Find line start */
    {
        QLineF line( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( src_item, src_item->shape().toFillPolygon());
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
    /** Find line end */
    if ( isNotNullP( dst_item))
    {
        QLineF line2( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( dst_item, dst_item->shape().toFillPolygon());
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
    prepareGeometryChange();
}

QRectF 
EdgeHelper::boundingRect() const
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
EdgeHelper::shape() const
{
    QPainterPath path( srcP);
    QPainterPathStroker stroker;
        
    if ( isNullP( src_item) 
         || srcP == dstP
         || state == HELPER_STATE_INITIAL)
        return path;
    
    if ( areEqP( src_item, dst_item))
    {
        path = selfEdgePath();
    } else
    {
        path.lineTo( dstP);
    }
    stroker.setWidth( 2);
    
    return stroker.createStroke( path); 
}

void 
EdgeHelper::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( isNullP( src_item) || state == HELPER_STATE_INITIAL)
        return;
    if ( option->levelOfDetail < 0.1)
        return;

    /** Do not draw edges when adjacent nodes intersect */
    if ( isNotNullP( dst_item) && areNotEqP( src_item, dst_item))
    {
        QPolygonF pred_rect = mapFromItem( src_item, src_item->boundingRect());
        QPolygonF succ_rect = mapFromItem( dst_item, dst_item->boundingRect());
        if ( !succ_rect.intersected( pred_rect).isEmpty())
            return;
    }

    static const qreal spline_detail_level = 0.4;
    static const qreal draw_arrow_detail_level = 0.3;
    QPointF curr_point;
    QLineF line( srcP, dstP);
    
    QPainterPath path( srcP);
    path.lineTo( dstP);
    if ( areEqP( src_item, dst_item))
    {
        path = selfEdgePath();
    } 
    if ( srcP == dstP)
        return;

    //Set opacity
    painter->setOpacity( 0.3);
    QPen pen( option->palette.foreground().color(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        
    // Draw the line itself
    painter->setPen( pen);
    painter->setBrush( option->palette.foreground().color());
    // Draw the arrows if there's enough room and level of detail is appropriate
    if ( option->levelOfDetail >= draw_arrow_detail_level)
    {
        double angle = ::acos(line.dx() / line.length());
        if ( line.dy() >= 0)
            angle = TwoPi - angle;
        
        QPointF destArrowP1;
        QPointF destArrowP2;

       
        /* NOTE:  Qt::black can be replaced by option->palette.foreground().color() */
        if ( areEqP( src_item, dst_item))
        {
            angle = -2* Pi/3;
        }  
        destArrowP1 = dstP + QPointF( sin(angle - Pi / 3) * arrowSize,
                                      cos(angle - Pi / 3) * arrowSize);
        destArrowP2 = dstP + QPointF( sin(angle - Pi + Pi / 3) * arrowSize,
                                      cos(angle - Pi + Pi / 3) * arrowSize);
        pen.setStyle( Qt::SolidLine);
        painter->setPen( pen);
        
        QPainterPath arrow_path;
        QPainterPathStroker stroker;
        path = stroker.createStroke( path);
        stroker.setWidth( 0);
        arrow_path.addPolygon( QPolygonF() << dstP << destArrowP1 << destArrowP2 <<  dstP);
        path = path.united( arrow_path);
        // path = path.simplified();
        //painter->drawPolygon(QPolygonF() << dstP << destArrowP1 << destArrowP2);
    }
    
    painter->drawPath( path);
}

