/**
 * @file: edge_helper.cpp 
 * Edge helper class implementation.
 * GUI for ShowGraph tool.
 */
/*
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
#include "gview_impl.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define QLINEF_INTERSECT intersects
#else
#define QLINEF_INTERSECT intersect
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define LEVELOFDETAIL(p,x) p->levelOfDetailFromTransform(x)
#else
#define LEVELOFDETAIL(p,x) p->levelOfDetail
#endif



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
                 polyLine.QLINEF_INTERSECT( line, &srcP);
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
                 polyLine.QLINEF_INTERSECT( line2, &dstP);
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
    if ( LEVELOFDETAIL(option, painter->worldTransform()) < 0.1)
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
    QPen pen( option->palette.windowText().color(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        
    // Draw the line itself
    painter->setPen( pen);
    painter->setBrush( option->palette.windowText().color());
    // Draw the arrows if there's enough room and level of detail is appropriate
    if ( LEVELOFDETAIL(option, painter->worldTransform()) >= draw_arrow_detail_level)
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

