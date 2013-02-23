/**
 * @file: graph.cpp
 * Graph class implementation
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Constructor.
 */
Graph::Graph( bool create_pools):
    node_next_id( 0),
    edge_next_id( 0),
    node_num( 0),
    edge_num( 0),
    first_node( NULL),
    first_edge( NULL),
    node_pool( NULL),
    edge_pool( NULL)
{
    QDomElement root = createElement("graph");
    appendChild( root);
    if ( create_pools)
        createPools();
}

/**
 * Destructor - removes all nodes
 */
Graph::~Graph()
{
    for ( Node *node = firstNode();
          isNotNullP( node);
          )
    {
        Node* next = node->nextNode();
        deleteNode( node);
        node = next;
    }
    destroyPools();
}

/** Pools' creation routine */
void Graph::createPools()
{
    node_pool = new FixedPool< Node>();
    edge_pool = new FixedPool< Edge>();
}

/** Pools' destruction routine */
void Graph::destroyPools()
{
    delete node_pool;
    delete edge_pool;
}

/**
 * Build graph from XML description
 */
void
Graph::readFromXML( QString txt) // changed to read from QString rather than from file
{

    if ( !setContent( txt))
    {
        GRAPH_ASSERTD( 0, "Not a good-formated xml file");
        return;
    }

    /**
     * Read nodes and create them
     */
    QDomElement docElem = documentElement();

    QDomNode n = docElem.firstChild();
    QHash< GraphUid, Node *> n_hash;

    while ( !n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        
        if ( !e.isNull() && e.tagName() == QString( "node"))
        {
            Node *node = newNode( e);
            node->readFromElement( e);
            n_hash[ e.attribute( "id").toLongLong()] = node;
        }
        n = n.nextSibling();
    }
    
    n = docElem.firstChild();
    while ( !n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        
        if ( !e.isNull() && e.tagName() == QString( "edge"))
        {
            GraphUid pred_id = e.attribute( "source").toLongLong();
            GraphUid succ_id = e.attribute( "target").toLongLong();
            Node *pred = n_hash[ pred_id];
            Node *succ = n_hash[ succ_id];
            Edge *edge = newEdge( pred, succ, e);
            edge->readFromElement( e);
        }
        n = n.nextSibling();
    }
}

/** Node/Edge creation routines can be overloaded by derived class */
Node * 
Graph::createNode( int _id)
{
    return new ( node_pool) Node ( this, _id);
}

Edge * 
Graph::createEdge( int _id, Node *_pred, Node* _succ)
{
    return new ( edge_pool) Edge( this, _id, _pred, _succ);
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
Graph::debugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
 * Implementation for numerations cleanup
 */
void 
Graph::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for ( n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( n);
    }
    /** Clean markers in edges */
    for ( e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedNumerations( e);
    }
}

/**
 * Implementation for markers cleanup
 */
void 
Graph::clearMarkersInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}

/**
 * Implementation of XML writing
 */
void 
Graph::writeToXML( QString filename)
{
    QFile file( filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        assert( 0);
        return;
    }
     
    /** Update element for each node */
    for ( Node *n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->updateElement();
    }

    /** Update element for each edge */
    for ( Edge *e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->updateElement();
    }

    QTextStream out( &file);
    save(out, IndentSize);
}
