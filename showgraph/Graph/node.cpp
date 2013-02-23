/**
 * @file: node.cpp
 * Node class implementation
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
/** 
 * Destructor. Corrects list of nodes in corresponding graph and deletes adjacent edges
 */
#include "graph_iface.h"

Node::~Node()
{
    Edge *edge;
    //out("Deleted Node");

    /** delete incidient edges */
    for ( edge = firstSucc(); isNotNullP( edge);)
    {
        Edge* next = edge->nextSucc();
        //edge->detachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        graph()->deleteEdge( edge);
        edge = next;
    }
    for ( edge = firstPred(); isNotNullP( edge);)
    {
        Edge* next = edge->nextPred();
        //edge->detachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        graph()->deleteEdge( edge);
        edge = next;
    }
    
    element.parentNode().removeChild( element);

    /** delete myself from graph */
    graph_p->detachNode( this);
}

/**
 * Print node in Dot format to stdout
 */
void
Node::debugPrint()
{
    out("%llu;", id());
}
