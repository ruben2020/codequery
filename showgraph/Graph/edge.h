/**
 * @file: edge.h
 * Edge class definition
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H

/**
 * Edge lists identificators
 * @ingroup GraphBase 
 */
enum EdgeListType
{
    EDGE_LIST_PREDS,
    EDGE_LIST_SUCCS,
    EDGE_LIST_GRAPH,
    EDGE_LISTS_NUM
};

/**
 * @class Edge
 * @brief  Representation of graph edge
 * @ingroup GraphBase
 *
 * @par
 * Edge class implements basic concept of graph edge. Every edge has two adjacent nodes.
 * Use pred() and succ() routines to get them. Edge is a member of 3 lists: edge list in graph,
 * pred list in succ node and succ list in pred node. To traverse these lists use nextEdge(),
 * nextPred() and  nextSucc() routines. Also for debug purposes all edges in a graph
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * @par
 * An edge can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref Graph "graph". Note that @ref Node "nodes" can be marked with the
 * same marker or numbered in the same numeration.
 * Also for debug purposes all nodes in a graph
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * @par
 * Edges reside in memory pool that is controlled by Graph. Operator new can't be called
 * directly. Edges can be only created by calling Graph::newEdge().
 *
 * @par
 * Every edge have associated QDomElement for XML export support. The updateElement() routine should be called before
 * export to get element in sync with edge's properties.
 *
 * @sa Graph
 * @sa Node
 * @sa Mark
 * @sa Nums
 */
class Edge: 
    public MListIface< Edge, // List item
                       MListItem< EDGE_LISTS_NUM>, // base class: pure multi-list item
                       EDGE_LISTS_NUM >, // Lists number                      
    public Marked,
    public Numbered,
    public PoolObj
{
public:
    /** Get edge's unique ID */
    inline GraphUid id() const;

    /** Get edge's graph */
    inline Graph * graph() const;

    /** 
     *  Destructor.
     *  Delete edge from list in graph.
     *  Deletion from node lists MUST be performed manually.
     */
    virtual ~Edge();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    inline void setNode( Node *n, GraphDir dir);
    
    /** Connect edge with given node as a predecessor */
    inline void setPred( Node *n);
    /** Connect edge with given node as a successor   */
    inline void setSucc( Node *n);

    /** Get node in specified direction  */
    inline Node *node( GraphDir dir) const;
    inline Node *pred() const;/**< Get predecessor node of edge */
    inline Node *succ() const;/**< Get successor node of edge   */

    /** Return next edge of the graph */
    inline Edge* nextEdge();

    /** Return next edge of the same node in given direction  */
    inline Edge* nextEdgeInDir( GraphDir dir);
    inline Edge* nextSucc();/**< Next successor */
    inline Edge* nextPred();/**< Next predecessor */
    
    /** Print edge in dot fomat to stdout */
    virtual void debugPrint();

    /** Return corresponding document element */
    inline QDomElement elem() const;

    /** Set document element */
    inline void setElement( QDomElement elem);

    /** Update DOM element */
    virtual void updateElement();

    /** Read properties from XML */
    virtual void readFromElement( QDomElement elem);

    /**
     * Insert a node on this edge
     *
     * Creates a node on edge and a new edge from new node to former successor of original edge.
     * Original edge goes to new node.
     * Return new node.
     */
    virtual Node *insertNode();
private:
    /** Representation in document */
    QDomElement element;

    /** Graph part */
    GraphUid uid; //Unique ID
    Graph * graph_p; //Graph

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    /** Node checking routine */
    bool checkNodes( Node* _pred, Node* _succ);

protected:
    /** Graph should have access to Edge's members */
    friend class Graph;
    /** Node should have access to Edge's members */
	friend class Node;

    /** Constructors are made private, only nodes and graph can create edges */
    Edge( Graph *_graph_p, GraphUid _id, Node *_pred, Node* _succ):
        uid(_id), graph_p(_graph_p)
    {
        GRAPH_ASSERTD( checkNodes( _pred, _succ),
                       "Predecessor and sucessor used in edge construction belong to different graphs");
        setPred( _pred);
        setSucc( _succ);
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    inline void detachFromNode( GraphDir dir);

    /**
	 * Remove myself from graph's list of edges
	 */
	inline void detachFromGraph()
    {
        detach( EDGE_LIST_GRAPH);
    }
};

#endif
