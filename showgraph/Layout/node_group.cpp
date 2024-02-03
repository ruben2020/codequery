/**
 * @file: node_group.cpp 
 * Implementation of node group class
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
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

#include <algorithm>
#include "layout_iface.h"

/**
 * Compare orders of nodes
 */
bool compareBc( AuxNode* node1,
                AuxNode* node2)
{
    if ( qFuzzyCompare( node1->bc(), node2->bc()))
    {
        return node1->order() < node2->order();
    }
    return ( node1->bc() < node2->bc());
}

/**
 * Constructor of group from a node.
 * Coordinates are computed with respect to pass direction
 */
NodeGroup::NodeGroup( AuxNode *n,   // Parent node
                      GraphDir dir, // Pass direction
                      bool first_pass) // If this is the first run
                      : node_list()
{
    init();
    addNode( n);
    /** Compute coordinates */
    double sum = 0;
    unsigned int num_peers = 0;
    /**
     * On descending pass we compute center coordinate with respect to coordinates of predecessors,
     * on ascending - we look at successors
     */
    GraphDir rdir = RevDir( dir);
    for ( AuxEdge* e = n->firstEdgeInDir( rdir);
          isNotNullP( e);
          e = e->nextEdgeInDir( rdir))
    {
        if ( !e->isInverted())
        {
            if ( !e->node( rdir)->isForPlacement())
                continue;
            num_peers++;
            if ( e->node( rdir)->isEdgeLabel())
            {
                sum+= ( e->node( rdir)->modelX());
            } else
            {
                sum+= ( e->node( rdir)->modelX() + ( e->node( rdir)->width() / 2));
            }
        }
    }
    for ( AuxEdge* e = n->firstEdgeInDir( dir);
          isNotNullP( e);
          e = e->nextEdgeInDir( dir))
    {
        if ( e->isInverted())
        {
            if ( !e->node( dir)->isForPlacement())
                continue;
            
            num_peers++;

            if ( e->node( dir)->isEdgeLabel())
            {
                sum+= ( e->node( dir)->modelX());
            } else
            {
                sum+= ( e->node( dir)->modelX() + ( e->node( dir)->width() / 2));
            }
        }
    }

    /** Barycenter heuristic */
    double center = 0;
    edge_num = 1;
    if ( num_peers > 0)
    {
        edge_num = num_peers;
        center = sum / num_peers;
    } else if ( !first_pass)
    {
        edge_num = 1;
        center = n->modelX() + n->width() / 2;
    }
    if ( n->isEdgeLabel())
        center += ( n->width() / 2);
    if ( n->isStable())
    {
        center = n->modelX() + n->width() / 2;
    }
    n->setBc( center);
    barycenter = center;
    
    border_left = center - n->width() / 2;
    border_right = center + n->width() / 2;
}

/**
 * Merge two groups correcting borders and nodes list of resulting group
 */
void NodeGroup::merge( NodeGroup *grp)
{
    /** Add nodes from group on the left */
    node_list += grp->nodes();
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;

    /** Recalculate border coordinates */
    /* 1. calculate center coordinate */
    qreal bc1 = bc();
    qreal bc2 = grp->bc();
    unsigned int e1 = adjEdgesNum();
    unsigned int e2 = grp->adjEdgesNum();
    qreal center = ( right() + left()) / 2; 
    center = ( bc1 * e1 + bc2 * e2) / (e1 + e2);
    
    /* 2. calculate width */
    qreal width = 0;
    int num = 0;
    qreal nodes_barycenter = 0;
    
    std::sort( node_list.begin(), node_list.end(), compareBc);
    
    foreach ( AuxNode* node, node_list)
    {
        width += node->spacing( prev_type);
        nodes_barycenter += width + node->width() / 2; 
        width += node->width();
        prev_type = node->type();
        num++;
    }
    if (num != 0) nodes_barycenter = nodes_barycenter / num;

    /* 3. set borders */
    setLeft( center - nodes_barycenter);
    setRight( left() + width);
    barycenter = center;
    edge_num = e1 + e2;
    
    //out("Width %e, center %e, barycenter %e", width, center, barycenter);

}

/**
 * Place nodes withing the group
 */
void NodeGroup::placeNodes()
{
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;
    
    qreal curr_left = left();
    foreach ( AuxNode* node, node_list)
    {
        curr_left += node->spacing( prev_type);
        node->setX( curr_left);
        curr_left += node->width();
        prev_type = node->type();
    }
}

/**
 * Place nodes withing the group
 */
void NodeGroup::placeNodesFinal( GraphDir dir)
{
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;
    
    qreal curr_left = left();
    //out("Node placement: from %e to %e", left(), right());

    foreach ( AuxNode* node, node_list)
    {
        //out("Node %d", node->id()); 
        curr_left += node->spacing( prev_type);
        node->setX( curr_left);
        curr_left += node->width();
        prev_type = node->type();
        node->setY( node->level()->y() - node->height() / 2 );
    }
}
