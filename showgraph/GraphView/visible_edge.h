/**
 * @file: visible_edge.h 
 * Definition of VEdge class
 * @ingroup GUIGraph
 */
/**
 * @page vedge Visible Edge Class
 * 
 * Visible Edge Class
 * VEdge stands for "visible edge". It is a class for manipulating
 * an edge like the user see it. The need for this stems from
 * the fact that edge seen by user is actually composed of 
 * several low-level edges between pseudo nodes.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef VISIBLE_EDGE_H
#define VISIBLE_EDGE_H

#include "gview_impl.h"

/**
 * Visible edge
 * @ingroup GUIGraph
 */
class VEdge
{
public:
    /** Constructor from two nodes */
    VEdge( GNode *pred_node, GNode *succ_node);
    /** Constructor from existing graph edge */
    VEdge( GEdge *prototype);
    
    /** Returns true if edge looks vertical in general */
    bool isVertical() const;
    /** Returns true if edge looks horizontal in general */
    bool isHorizontal() const;
    
    /** Get end node that is in 'up' direction, returns NULL if edge is horizontal */
    GNode *nodeUp() const;
    /** Get end node that is in 'down' direction, returns NULL if edge is horizontal */
    GNode *nodeDown() const;
    /** Get end node that is in 'left' direction, returns NULL if edge is vertical */
    GNode *nodeLeft() const;
    /** Get end node that is in 'right' direction, returns NULL if edge is vertical */
    GNode *nodeRight() const;
    
    /** Get pred */
    inline GNode* pred() const;
    /** Get succ */
    inline GNode *succ() const;
private:    
    GNode *pred_priv;
    GNode *succ_priv;
};

/** Get pred */
GNode* VEdge::pred() const
{
    return pred_priv;
}
/** Get succ */
GNode *VEdge::succ() const
{
    return succ_priv;
}

#endif /** VISIBLE_EDGE_H */