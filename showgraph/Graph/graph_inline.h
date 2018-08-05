/**
 * @file: graph_inline.h
 * Implementation of Graph's inline routines
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
#pragma once
#ifndef GRAPH_INLINE_H
#define GRAPH_INLINE_H

/** Delete node from memory pool */
inline void Graph::deleteNode( void *n)
{
    node_pool->destroy( n);
}

/** Delete edge from memory pool */
inline void Graph::deleteEdge( void *e)
{
    edge_pool->destroy( e);
}

/**
 * Remove node from node list of graph
 */
inline void Graph::detachNode( Node* node)
{
    assertd( isNotNullP( node));
    assertd( node->graph() == this);

    if( first_node == node)
    {
       first_node = node->nextNode();
    }
    node->detachFromGraph();
    node_num--;
}

/**
 * Remove edge from edge list of graph
 */
inline void Graph::detachEdge( Edge * edge)
{
    assertd( isNotNullP( edge));
    assertd( edge->graph() == this);

    if( first_edge == edge)
    {
       first_edge = edge->nextEdge();
    }
    edge->detachFromGraph();
    edge_num--;
}

/**
 * Return node quantity
 */
inline GraphNum Graph::nodeCount() const
{
    return node_num;
}

/**
 * Return edge quantity
 */
inline GraphNum Graph::edgeCount() const
{
    return edge_num;
}
/** 
 * Get first edge
 */
inline Edge* Graph::firstEdge() 
{
    return first_edge;
}

/**
 * Get first node
 */
inline Node* Graph::firstNode()
{
    return first_node;
}

/** Get pool of nodes */
inline Pool *Graph::nodePool() const
{
    return node_pool;
}
/** Get pool of edges */
inline Pool *Graph::edgePool() const
{
    return edge_pool;
}

/**
 * Creation node in graph implementation
 */
inline Node * 
Graph::newNodeImpl( GraphUid id)
{
    /**
     * Check that we have available node id 
     */
    assertd( edge_next_id < GRAPH_MAX_NODE_NUM);
    
    /** Create node */
    Node *node_p = this->createNode( id);
    
    /** Add node to graph's list of nodes */
    node_p->attach( first_node);
    first_node = node_p;
    
    node_num++;
    
    /** Make sure that next automatically assigned id will be greater than given id */
    if ( node_next_id <= id)
        node_next_id = id + 1;
    return node_p;
}

/**
 * Creation node in graph
 */
inline Node * 
Graph::newNode()
{
    Node * node_p = newNodeImpl( node_next_id);
    node_p->setElement( createElement( "node"));
    documentElement().appendChild( node_p->elem());
    return node_p;
}


/**
 * Creation node in graph
 */
inline Node * 
Graph::newNode( QDomElement e)
{
    assertd( !e.isNull());
    assertd( e.tagName() == QString( "node"));
    assertd( e.hasAttribute( "id"));

    Node *node_p = newNodeImpl( node_next_id);
    node_p->setElement( e);
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
inline Edge * 
Graph::newEdgeImpl( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    assertd( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = this->createEdge( edge_next_id++, pred, succ);
    edge_p->attach( EDGE_LIST_GRAPH, first_edge);
    first_edge = edge_p;
    edge_num++;
    return edge_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
inline Edge * 
Graph::newEdge( Node * pred, Node * succ)
{
    Edge *edge_p = newEdgeImpl( pred, succ);
    edge_p->setElement( createElement( "edge"));
    documentElement().appendChild( edge_p->elem());
    return edge_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
inline Edge * 
Graph::newEdge( Node * pred, Node * succ, QDomElement e)
{
    Edge *edge_p = newEdgeImpl( pred, succ);
    edge_p->setElement( e);
    return edge_p;
}


#endif /** GRAPH_INLINE_H */
