/**
 * @file: visible_edge.cpp 
 * Implemetation of VEdge class
 * @ingroup GUIGraph
 */
/*
 * GUI for ShowGraph tool.
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

/** Maximal contangent for edge to be considered 'vertical' */
const qreal MAX_COT_VERTICAL = 0.1;
/** Maximal tangent for edge to be considered 'horizontal' */
const qreal MAX_TAN_HORIZONTAL = 0.1;

/** Constructor from two nodes */
VEdge::VEdge( GNode *pred_node, GNode *succ_node):
    pred_priv( pred_node),
    succ_priv( succ_node)
{

}

/** Constructor from existing graph edge */
VEdge::VEdge( GEdge *prototype)
{
    pred_priv = prototype->realPred();
    succ_priv = prototype->realSucc();
}

/** Returns true if edge looks vertical in general */
bool VEdge::isVertical() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    qreal delta_x = abs<qreal>( p_center.x() - s_center.x());
    qreal delta_y = abs<qreal>( p_center.y() - s_center.y());

    if ( delta_x/delta_y > MAX_COT_VERTICAL)
        return false;
    return true;
}

/** Returns true if edge looks horizontal in general */
bool VEdge::isHorizontal() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    qreal delta_x = abs<qreal>( p_center.x() - s_center.x());
    qreal delta_y = abs<qreal>( p_center.y() - s_center.y());

    if ( delta_y/delta_x > MAX_TAN_HORIZONTAL)
        return false;
    return true;
}

/** Get end node that is in 'up' direction, returns NULL if edge is horizontal */
GNode * 
VEdge::nodeUp() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    
    if ( isHorizontal())
        return NULL;
    // Note: y axis goes downwards
    if ( p_center.y() > s_center.y()) 
    {
        return succ();
    } else
    {
        return pred();
    }
}
/** Get end node that is in 'down' direction, returns NULL if edge is horizontal */
GNode * 
VEdge::nodeDown() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    
    if ( isHorizontal())
        return NULL;
    // Note: y axis goes downwards
    if ( p_center.y() > s_center.y()) 
    {
        return pred();
    } else
    {
        return succ();
    }
}

/** Get end node that is in 'left' direction, returns NULL if edge is vertical */
GNode * 
VEdge::nodeLeft() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    
    if ( isVertical())
        return NULL;
    // Note: x axis goes right
    if ( p_center.x() > s_center.x()) 
    {
        return succ();
    } else
    {
        return pred();
    }
}

/** Get end node that is in 'right' direction, returns NULL if edge is vertical */
GNode * 
VEdge::nodeRight() const
{
    NodeItem *p_item = pred()->item();
    NodeItem *s_item = succ()->item();
    
    // Both points in p_item's coordinates
    QPointF p_center = p_item->boundingRect().center();
    QPointF s_center = p_item->mapFromItem( s_item, s_item->boundingRect().center());
    
    if ( isVertical())
        return NULL;
    // Note: x axis goes right
    if ( p_center.x() > s_center.x()) 
    {
        return pred();
    } else
    {
        return succ();
    }
}
