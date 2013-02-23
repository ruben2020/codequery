/**
 * @file: aux_graph.cpp
 * Implementation of auxiliary graph
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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