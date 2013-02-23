/**
 * @file: graph.h 
 * Graph class definition/implementation.
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

/**
 * @class Graph
 * @brief Basic representation of graph
 * @ingroup GraphBase
 *
 * @par 
 * The Graph class represents graph as a whole. As one can expect graph has @ref Node "nodes"
 * and @ref Edge edges which impement the directed graph data structure. For traversing nodes
 * and edges they are linked in two lists. One can traverse these lists using firstNode() and firstEdge()
 * routines with calling Node::nextNode() Edge::nextEdge() in a loop.
 * Example:
 * @code
 //Traversing nodes
 for ( Node *n = firstNode(); isNotNullP( n); n = n->nextNode())
 {
     ...
 }
 //Traversing edges
 for ( Node *e = firstEdge(); isNotNullP( e); e = e->nextEdge())
 {
     ...
 }
 @endcode
 * The same result can be achieved using macros 
 * @code
 
 // Graph *g; Node *n; Edge *e;
 // Traversing nodes
 foreachNode( n, g)
 {
    ...
 }
 
 // Traversing edges
 foreachEdge( e, g)
 {
    ...
 }
 @endcode
 *
 * @par
 * The graph is also owner of memory allocated for its nodes and edges. This is
 * implemented via @ref FixedPool "memory pools" with records of fixed size. Nodes and
 * Edges should be created through newNode() and newEdge() routines. They can be deleted by
 * deleteNode() and deleteEdge() routines. Do not use operators new/delete for graph's 
 * nodes and edges.
 * 
 * @par
 * Graph is also manager of @ref Mark "markers" and @ref Nums "numerations" for nodes and edges.
 * New @ref Marker "marker" can be obtained by newMarker() routine. New @ref Numeration "numeration" is
 * created by newNum().
 * Example:
 * @code
 //Graph *graph; Node *n;
 Marker m = graph.newMarker();
 Numeration num = graph.newNum();
    
 GraphNum i = 0; //unsigned int 32
 //Mark nodes without predecessors
 foreachNode( n, g)
 {
    if ( isNullP( n->firstPred()))
    {
        n->mark( m);
        n->setNumber( num, i++);
    }
 }
 ...

 //Checking markers and numbers
 foreachNode( n, g)
 {
     if ( n->isMarked( m) && n->number( num) > 10)
     {
        ...
     }
 }
 graph->freeMarker( m);
 graph->freeNum( num);

 @endcode
 *
 * @par Deriving classes from Graph
 * To make a graph-like data structure one can use Graph as a base class. Most likely
 * it will be also necessary to implement two more classes to make a useful implementation.
 * These two class should be derived from Node and Edge to represent information  
 * associated with nodes and edges. You can see an example of such inheritance in AGraph,
 * ANode and AEdge classes.
 *
 * @sa Node
 * @sa Edge
 * @sa Mark
 * @sa Nums
 * @sa AGraph
 */
class Graph: public MarkerManager, public NumManager, public QDomDocument
{
public:
    /**
     * Constructor. 
     * Derived classes may be call constructor with 'false' vaule of
     * the parameter to prevent pools creation for base-level nodes and edges.
     * In this case pool should be created by derived class itself.
     */
    Graph( bool create_pools);
    
    /** Destructor */
    virtual ~Graph();

    /** Create new node in graph */
    inline Node * newNode();

    /** Create new node in graph and fills it with info in element */
    inline Node * newNode( QDomElement e);

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    inline Edge * newEdge( Node * pred, Node * succ);
    /**
     * Create edge between two nodes from an XML description
     * We do not support creation of edge with undefined endpoints
     */
	inline Edge * newEdge( Node * pred, Node * succ, QDomElement e);
    
    /** 
     *  Delete node. Substitution for node's operator delete, which shouldn't
     *  be called directly since Node is a pool-residing object
     */
    inline void deleteNode( void *n);

    /** 
     *  Delete node. Substitution for edges's operator delete, which shouldn't
     *  be called directly since Edge is a pool-residing object
     */
    inline void deleteEdge( void *e);
    
    /** Remove node from node list of graph */
    inline void detachNode( Node* node);

    /** Remove edge from edge list of graph */
    inline void detachEdge( Edge * edge);

    /** Return number of nodes in graph */
    inline GraphNum nodeCount() const;

    /** Return number of edges in graph */
    inline GraphNum edgeCount() const;
    
    /** Get first edge */
    inline Edge* firstEdge();

    /** Get first node */
    inline Node* firstNode();
    
    /** Print graph to stdout in DOT format */
    virtual void debugPrint();
 
    /**
     * Save graph as an XML file
     */
    virtual void writeToXML( QString filename);

    /**
     * Build graph from XML description
     */
    virtual void readFromXML( QString txt);

protected:

    /** Node creation routine is to be overloaded by derived class */
	virtual Node * createNode( int _id);
	/** Edge creation routine is to be overloaded by derived class */
    virtual Edge * createEdge( int _id, Node *_pred, Node* _succ);
    
    /** Pools' creation routine */
    virtual void createPools();
    /** Pools' destruction routine */
    virtual void destroyPools();

    /** Get pool of nodes */
    inline Pool *nodePool() const;
    /** Get pool of edges */
    inline Pool *edgePool() const;

    /** Memory pool for nodes */
    Pool *node_pool;
    /** Memory pool for edges */
    Pool *edge_pool;
private:
    /**
     * Implementation of node creation
     */
    inline Node * newNodeImpl( GraphUid id);
    /**
     * Implementation of edge creation
     */
	inline Edge * newEdgeImpl( Node * pred, Node * succ);

    /** Clear unused markers from marked objects */
    void clearMarkersInObjects();

    /** Clear unused numerations from numbered objects */
    void clearNumerationsInObjects();

    /** First node */
    Node* first_node;
    /** Number of nodes */
    GraphNum node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /* List of edges and its iterator */
    Edge* first_edge;
    GraphNum edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphUid edge_next_id;
};

#endif
