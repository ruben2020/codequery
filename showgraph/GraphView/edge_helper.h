/**
 * @file: edge_item.h 
 * Edge Widget class definition.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_HELPER_H
#define EDGE_HELPER_H

#include "gview_impl.h"

/**
 * states of edge helper
 */
enum EdgeHelperState
{
    /* Initial state: mouse didn't move out of src node */
    HELPER_STATE_INITIAL,
    /* regular state */
    HELPER_STATE_REGULAR
};
/**
 * Graphics item for visualizing an edge prototype so the user can see the future
 * edge during the creation process ( when he/she moves mouse holidng right button)
 * @ingroup GUIGraph
 */
class EdgeHelper: public QGraphicsItem
{
    QPointF srcP;
    QPointF dstP;
    QPointF cp1;
    QPointF cp2;
    QPointF topLeft;
    QPointF btmRight;
    NodeItem *src_item;
    NodeItem *dst_item;
    EdgeHelperState state;
public:
    /** Type of graphics item for edge */
    enum {Type = TypeEdgeHelper};
    /** Constructor */
    EdgeHelper();
    
    /** Get item type */
    inline int type() const;
    /** Set source item */
    inline void setSrc( NodeItem *item);
    /** Set destination item */
    inline void setDst( NodeItem *item);
    /** Get source item */
    inline NodeItem *src() const;
    /** Get destination item */
    inline NodeItem *dst() const;
    /** Set destination point */
    inline void setDstP( QPointF p);
    /** Reset state */
    inline void reset();
    /** promote state to regular */
    inline void switchToRegular();
    /** Adjust edge image */
    void adjust();
    /** Get bounding rectangle of edge image */
    QRectF boundingRect() const;
    /** Get shape of the edge */
    QPainterPath shape() const;
    /** Paint edge */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /** Convenience routine for self edge path */
    QPainterPath selfEdgePath() const;
};

/** promote state to regular */
inline void 
EdgeHelper::switchToRegular()
{
    state = HELPER_STATE_REGULAR;
}

/** Reset state */
inline void
EdgeHelper::reset()
{
    src_item = NULL;
    dst_item = NULL;
    dstP = QPointF( 0, 0);
    state = HELPER_STATE_INITIAL;
}

/** Get item type */
inline int EdgeHelper::type() const
{
    return Type;
}

/** Set source item */
inline void
EdgeHelper::setSrc( NodeItem *item)
{
    src_item = item;
}
/** Set destination item */
inline void
EdgeHelper::setDst( NodeItem *item)
{
    dst_item = item;
}
/** Get source item */
inline NodeItem *
EdgeHelper::src() const
{
    return src_item;
}
/** Get destination item */
inline NodeItem *
EdgeHelper::dst() const
{
    return dst_item;
}
/** Set destination point */
inline void
EdgeHelper::setDstP( QPointF p)
{
    dstP = p;
}

#endif /* EDGE_HELPER_H */
