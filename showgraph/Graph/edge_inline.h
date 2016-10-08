/**
 * @file: edge.h
 * Implementation of Edge class inline routines
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
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
#ifndef EDGE_INLINE_H
#define EDGE_INLINE_H

/**
 * Low level correction of node's edge list in corresponding direction
 */
inline void
Edge::detachFromNode( GraphDir dir)
{
    if ( isNotNullP( node( dir)))
    {
        Node *n = node( dir);
        n->deleteEdgeInDir( RevDir( dir), (Edge* )this);
        detach( RevDir( dir));
        nodes[ dir] = 0;
    }
}

/**
 * Connect edge to a node in specified direction.
 * Note that node treats this edge in opposite direction. I.e. an edge that has node in
 * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
 */
inline void 
Edge::setNode( Node *n, GraphDir dir)
{
    assert( isNotNullP( n));
    nodes[ dir] = n;
    if ( n != NULL)
    {
        n->AddEdgeInDir( (Edge *)this, 
            ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
    }
}

/**
 * Return corresponding document element
 */
inline QDomElement Edge::elem() const
{
    return element;
}

/**
 * Set document element
 */
inline void Edge::setElement( QDomElement elem)
{
    element = elem;
}

/**
 * Get edge's unique ID
 */
inline GraphUid Edge::id() const
{
    return uid;
}

/**
 * Get edge's corresponding graph
 */
inline Graph * Edge::graph() const
{
    return graph_p;
}

/**
 * Connect edge with given node as a predecessor
 */
inline void Edge::setPred( Node *n)
{
    setNode( n, GRAPH_DIR_UP);
}
/**
 * Connect edge with given node as a successor
 */
inline void Edge::setSucc( Node *n)
{
    setNode( n, GRAPH_DIR_DOWN);
}

/**
 * Get node in specified direction
 */
inline Node *Edge::node( GraphDir dir) const
{
    return nodes[ dir];
}
/**
 * Get predecessor of edge
 */
inline Node *Edge::pred() const
{
    return node( GRAPH_DIR_UP);
}
/**
 * Get successor of edge
 */
inline Node *Edge::succ() const
{
    return node( GRAPH_DIR_DOWN);
}

/**
 * Return next edge of the graph
 */
inline Edge* Edge::nextEdge()
{
    return next( EDGE_LIST_GRAPH);
}

/**
 * Return next edge of the same node in given direction
 */
inline Edge* Edge::nextEdgeInDir( GraphDir dir)
{
    GRAPH_ASSERTD( dir < GRAPH_DIRS_NUM, "Wrong direction parameter");
    GRAPH_ASSERTD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    return next( dir);
}

/**
 * Next successor
 */
inline Edge* Edge::nextSucc()
{
    return nextEdgeInDir( GRAPH_DIR_DOWN);
}

/**
 * Next predecessor
 */
inline Edge* Edge::nextPred()
{
    return nextEdgeInDir( GRAPH_DIR_UP);
}

/**
 * Insert a node on this edge
 *
 * Creates a node on edge and a new edge from new node to former successor of original edge.
 * Original edge goes to new node. 
 * Return new node.
 */
inline Node *
Edge::insertNode()
{
    Node *tmp_succ = succ();
    Node *new_node = graph()->newNode();
    detachFromNode( GRAPH_DIR_DOWN);
    setSucc( new_node);
    graph()->newEdge( new_node, tmp_succ);
    return new_node;
}


#endif