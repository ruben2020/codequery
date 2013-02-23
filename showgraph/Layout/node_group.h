/**
 * @file: node_group.h
 * Group of nodes
 *
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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