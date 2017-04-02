/**
 * @file: aux_graph.cpp
 * Implementation of auxiliary graph
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
#include "layout_iface.h"

/** Destructor for node */
AuxNode::~AuxNode()
{

}

/**
 * Default constructor
 */
AuxGraph::AuxGraph( bool create_pools):
    Graph( false),
    ranking_valid( false),
    levels(),
    cur_level(0),
    cur_pass(0),
    max_rank(0),
    layout_in_process( false) 
{
    ranking = newNum();
    order = newNum();
    
    watcher = new QFutureWatcher<void>();
    
    /** Pools' creation routine */
    if ( create_pools)
    {
        node_pool = new FixedPool< AuxNode>();
        edge_pool = new FixedPool< AuxEdge>();
    }
 
    connect( watcher, SIGNAL(finished()), this, SLOT( layoutNextStep()));
}

/**
 * Initialize levels
 */
void
AuxGraph::initLevels( Rank max_level)
{
    levels.resize( max_level + 1);
    for ( Rank i = 0; i <= max_level; i++)
    {
        levels[ i] = new Level( i);
    } 
}

/**
 * Delete levels
 */
void
AuxGraph::deleteLevels()
{
    for ( int i = 0; i < levels.size(); i++)
    {
        delete levels[ i];
    } 
}


/**
 * Destructor. Cleans up level info
 */
AuxGraph::~AuxGraph()
{
    deleteLevels();
    delete watcher;
    freeNum( ranking);
    freeNum( order);
}

/**
 * Get root node of the graph
 */
AuxNode*
AuxGraph::rootNode()
{
    Level* root_level = levels[ 0];

    if( isNullP( root_level) || isNullP( firstNode()))
    {
        return firstNode();
    }
    /** Use median heuristic to select root node */
    qreal center = 0;
    foreach ( AuxNode* node, root_level->nodes())
    {
        center = node->modelX() + node->width()/ 2;
    }
    center = center / root_level->nodes().count();
    
    AuxNode* prev = NULL;
    AuxNode* node = NULL;
    
    foreach ( node, root_level->nodes())
    {
        if ( center <= node->modelX() + node->width()/ 2)
        {
            qreal delta_curr = node->modelX() + node->width()/ 2 - center;
            if ( isNotNullP( prev)
                 && ( delta_curr > center - prev->modelX() - prev->width()/ 2))
            {
                return prev;
            } else
            {
                return node;
            }
        }
        prev = node;
    }
    return firstNode();
}
