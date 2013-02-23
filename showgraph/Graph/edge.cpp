/**
 * @file: edge.cpp 
 * Edge class implementation
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Edge destructor.
 * delete edge from graph's list of edges
 */
Edge::~Edge()
{
    //out("Deleted edge");
    element.parentNode().removeChild( element);
    graph_p->detachEdge( this);
    detachFromNode( GRAPH_DIR_UP);
    detachFromNode( GRAPH_DIR_DOWN);
}

/**
 * Print edge in DOT format to stdout
 */
void
Edge::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    assert( isNotNullP( pred()));
    assert( isNotNullP( succ()));

    out("%llu->%llu;", pred()->id(), succ()->id());
}

/**
 * Update DOM tree element
 */

void
Edge::updateElement()
{
    element.setAttribute( "source", pred()->id());
    element.setAttribute( "target", succ()->id());
}

/**
 * read properties from DOM tree element
 */
void
Edge::readFromElement( QDomElement e)
{
    element = e;
}

/** Node checking routine */
bool Edge::checkNodes( Node* _pred, Node* _succ)
{
    return isNotNullP( _pred)
           && isNotNullP( _succ)
           && areEqP( this->graph(), _pred->graph())
           && areEqP( _pred->graph(), _succ->graph());
}
