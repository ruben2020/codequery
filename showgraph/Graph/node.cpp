/**
 * @file: node.cpp
 * Node class implementation
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
