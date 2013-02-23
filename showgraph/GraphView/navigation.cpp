/**
 * @file: navigation.cpp 
 * Implemetation of navigation helper classes
 * @ingroup GUIGraph
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "gview_impl.h"

/** Constructor */
NodeNav::NodeNav( GNode *curr_node, NavSector nav_sector):
    node_priv( curr_node),
    sector_priv( nav_sector)
{

}
    
/** Get edge to the bottom of current node */
GEdge *
NodeNav::firstEdgeInSector() const
{
    GEdge *res = NULL;
    qreal min_delta = 0;

    /** For each edge */   
    for ( Node::EdgeIter e_iter = node()->edgesBegin(),
                         e_end = node()->edgesEnd();
          e_iter != e_end;
          e_iter++ )
    {
        GEdge *e = static_cast<GEdge *>( e_iter.edge());
        if ( isEdgeInSector( e))
        {
            NodeItem *p_item = static_cast<GNode *>( e_iter.node())->item();
            QPointF center = node()->item()->boundingRect().center();
            QPointF point = node()->item()->mapFromItem( p_item, p_item->boundingRect().center());
            
            qreal delta;
            if ( sector() == TOP_SECTOR || sector() == BOTTOM_SECTOR)
            {
                delta = abs<qreal>( center.x() - point.x());
            } else
            {
                delta = abs<qreal>( center.y() - point.y());
            }

            if ( isNullP( res) 
                 || delta < min_delta) // for selection of closest edge
            {
                res = e;
                min_delta = delta;
            }
        }
    }
    return res;
}

/** Checks if navigation direction is applicable in current sector */
bool NodeNav::isDirApplicable( NavDirection dir, NavSector s)
{
    /* Applicable only for top and bottom sectors */
    if ( s == UNDEF_SECTOR)
        return false;

    if ( s == LEFT_SECTOR
         || s == RIGHT_SECTOR)
    {
        return dir == NAV_DIR_UP
               || dir == NAV_DIR_DOWN;
    } else if ( s == TOP_SECTOR
                || s == BOTTOM_SECTOR)
    {
        return dir == NAV_DIR_LEFT
               || dir == NAV_DIR_RIGHT;
    }
    GVIEW_ASSERTD( 0, "Invalid sector type");
    return false;
}

/**
 * Check that given point is in given direction from reference point
 */
bool NodeNav::isPointInDir( QPointF point, QPointF ref, NavDirection dir)
{
    switch ( dir)
    {
    case NAV_DIR_UP: 
        return point.y() <= ref.y();
    case NAV_DIR_DOWN: 
        return point.y() >= ref.y();
    case NAV_DIR_LEFT:
        return point.x() <= ref.x();
    case NAV_DIR_RIGHT:
        return point.x() >= ref.x();
    default:
        return false;
    }
    return false;
}

/**
 * Compute coordinate difference in given direction 
 * between given point and reference point
 */
qreal NodeNav::deltaInDir( QPointF point, QPointF ref, NavDirection dir)
{
    switch ( dir)
    {
    case NAV_DIR_UP: 
       return ref.y() - point.y();
    case NAV_DIR_DOWN: 
        return point.y() - ref.y();
    case NAV_DIR_LEFT: 
        return ref.x() - point.x();
    case NAV_DIR_RIGHT:
        return point.x() - ref.x();
    default:
        return 0;
    }
    return 0;
}
/** Get edge to the left of given edge */
GEdge *
NodeNav::edgeInDir( GEdge * edge, NavDirection dir) const
{
    /* Applicable only for top and bottom sectors */
    if ( !isDirApplicable( dir, sector()))
    {
        return NULL;
    }
    
    GNode *n = otherEnd( edge);
    /* item corresponding to other (than node_priv) node */
    if ( isNullP( n))
        return NULL;

    NodeItem *item = n->item();
    
    // in current node's coordinates
    QPointF edge_point = node()->item()->mapFromItem( item, item->boundingRect().center());

    GEdge *res = NULL;
    qreal min_delta = 0;
    /** For each edge */   
    for ( Node::EdgeIter e_iter = node()->edgesBegin(),
                         e_end = node()->edgesEnd();
          e_iter != e_end;
          e_iter++ )
    {
        GEdge *e = static_cast<GEdge *>( e_iter.edge());
        if ( isEdgeInSector( e) && areNotEqP( e, edge))
        {
            NodeItem *p_item = static_cast<GNode *>( e_iter.node())->item();
            QPointF point = node()->item()->mapFromItem( p_item, p_item->boundingRect().center());
            if ( isPointInDir( point, edge_point, dir))
            {
                qreal delta = deltaInDir( point, edge_point, dir);
                if ( isNullP( res) 
                     || delta < min_delta) // for selection of closest edge
                {
                    res = e;
                    min_delta = delta;
                }
            }
        }
    }

    return res;
}

/* Other edge's node */
GNode *
NodeNav::otherEnd( GEdge *edge) const
{
    if ( areEqP( node_priv, edge->pred()))
    {
        return edge->succ();
    } else if ( areEqP( node_priv, edge->succ()))
    {
        return edge->pred();
    }
    return NULL;
}

/** Check that given edge is in current sector */
bool NodeNav::isEdgeInSector( GEdge * edge) const
{
    GNode *n = otherEnd( edge);
    /* item corresponding to other (than node_priv) node */
    if ( isNullP( n) || sector() == UNDEF_SECTOR)
        return false;

    NodeItem *item = n->item();
    
    // Both points in current node's coordinates
    QPointF p_center = node_priv->item()->boundingRect().center();
    QPointF s_center = node_priv->item()->mapFromItem( item, item->boundingRect().center());

    qreal angle = QLineF( p_center, s_center).angle();
    
    qreal max_angle = sectorMaxAngle();
    qreal min_angle = sectorMinAngle();

    if ( angle > 270 && sector() == RIGHT_SECTOR)
    {
        max_angle +=360;
    }
    if ( angle < 90 && sector() == RIGHT_SECTOR)
    {
        min_angle -=360;
    }

    if ( angle <= max_angle && angle >= min_angle)
        return true;
    return false;
}

/** Compute max angle for sector */
qreal NodeNav::sectorMaxAngle() const
{
    QLineF line;
    QRectF rect = node_priv->item()->boundingRect();
    QPointF center = rect.center();
    
    switch ( sector_priv)
    {
        case TOP_SECTOR:
            line = QLineF( center, rect.topLeft());
            break;
        case BOTTOM_SECTOR:
            line = QLineF( center, rect.bottomRight());
            break;
        case LEFT_SECTOR:
            line = QLineF( center, rect.bottomLeft());
            break;
        case RIGHT_SECTOR:
            line = QLineF( center, rect.topRight());
            break;
        case UNDEF_SECTOR:
        default:
            return 0;
    }
    return line.angle();
}

/** Compute min angle for sector */
qreal NodeNav::sectorMinAngle() const
{
    QLineF line;
    QRectF rect = node_priv->item()->boundingRect();
    QPointF center = rect.center();
    
    switch ( sector_priv)
    {
        case TOP_SECTOR:
            line = QLineF( center, rect.topRight());
            break;
        case BOTTOM_SECTOR:
            line = QLineF( center, rect.bottomLeft());
            break;
        case LEFT_SECTOR:
            line = QLineF( center, rect.topLeft());
            break;
        case RIGHT_SECTOR:
            line = QLineF( center, rect.bottomRight());
            break;
        case UNDEF_SECTOR:
        default:
            return 0;
    }
    return line.angle();
}