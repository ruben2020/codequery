/**
 * @file: node_group.h
 * Group of nodes
 *
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
 * @defgroup HLayout Horizontal placement
 * @ingroup Layout
 */
#ifndef NODE_GROUP_H
#define NODE_GROUP_H

/**
 * Group of nodes
 *
 * Abstraction that aids multiple nodes manipulation inside a level.
 *
 * @ingroup HLayout
 */
class NodeGroup
{
    /** Number of edges in adjacent layer */
    unsigned int edge_num;
    /** Barycenter */
    qreal barycenter;
    /** List of group's nodes */
    QList< AuxNode *> node_list;
    /** Horizontal coordinate of left border */
    qreal border_left;
    /** Horizontal coordinate of right border */
    qreal border_right;
    /** The heuristic weight of the group */
    float group_weight;
public:
    /** Initialize attributes of the group */
    inline void init()
    {
        border_left = 0;
        border_right = 0;
        group_weight = 0;        
    }
    /** Get adjacent number of adjacent edges */
    inline unsigned int adjEdgesNum() const
    {
        return edge_num;
    }
    /** Get barycenter of adjacent edges */
    inline qreal bc() const
    {
        return barycenter;
    }
    /** Get weight */
    inline float weight() const
    {
        return group_weight;
    }
    /** Get left border coordinate */
    inline qreal left() const
    {
        return border_left;
    }
    /** Get right border coordinate */
    inline qreal right() const
    {
        return border_right;
    }
    /** Set left border coordinate */
    inline void setLeft( qreal pos)
    {
        border_left = pos;
    }
    /** Set right border coordinate */
    inline void setRight( qreal pos)
    {
        border_right = pos;
    }
    /** Get node list */
    QList<AuxNode *> nodes() const
    {
        return node_list;
    }

    /** Add node to list */
    inline void addNode( AuxNode *node)
    {
        node_list.push_back( node);
        group_weight += 1;
    }

    /** Default constructor */
    NodeGroup() : node_list()
    {
        init();
    }

    /** Constructor of group from a node */
    NodeGroup( AuxNode *n, GraphDir dir, bool first_pass);

    /** Check if this groups interleaves with the given one */
    inline bool interleaves( NodeGroup *grp) const
    {
        return !( left() > grp->right() 
                  || right() < grp->left());
    }
    
    /** Merge two groups correcting borders and nodes list of resulting group */
    void merge( NodeGroup *grp);

    /** Place nodes inside group */
    void placeNodes();
    /** Place nodes and adjust the view */
    void placeNodesFinal( GraphDir dir);
};
#endif