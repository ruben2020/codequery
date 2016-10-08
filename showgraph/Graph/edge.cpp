/**
 * @file: edge.cpp 
 * Edge class implementation
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
