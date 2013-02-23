/**
 * @file: node.h 
 * Node class definition
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef NODE_H
#define NODE_H

#include "node_iter.h"

/**
 * @class Node
 * @brief Representation of graph node. 
 *
 * @ingroup GraphBase
 * 
 * @par
 * A graph node has two lists of edges which represent predecessors and successors. 
 * Node's predecessors and successors can be traversed by using three interfaces:
 * -# Get first edge in direction of interest via firstSucc(),firstPred() and then 
 *    use Edge's interface Edge::nextSucc(), Edge::nextPred()
 * -# Use iterators Node::Succ and Node::Pred wich are used for successor and predecessor traversal of edges. 
 *    Routines succsBegin(), succsEnd(), predsBegin() and predsEnd() are used for creating iterators.
 * -# EdgeIter can be used for iterating trough all node's adjacent edges without respect to 
 *    their direction.
 * 
 * @code
  // Traversing edges simple
  for ( Edge* e = firstPred();
        isNotNullP( e);
        e = e->nextPred())
  {
     ...
  }
  //Same using macro foreachPred
  Edge* e; 
  foreachPred( e, node)
  {
     ...
  }
  //Traversal via iterators
  for ( Node::Succ s = node->succsBegin(),
                   s_end = node->succsEnd();
        s != s_end;
        s++ )
  {
     ...
  }
  @endcode
 * @par
 * A node can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref Graph "graph". Note that @ref Edge "edges" can be marked with the
 * same marker or numbered in the same numeration.
 * 
 * @par
 * All nodes in graph are linked in a list. Previous and next nodes can be obtained
 * through prevNode and nextNode routines. Also for debug purposes all nodes in a graph
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * @par
 * A node resides in memory pool that is controlled by Graph. Operator new can't be called 
 * directly. Nodes can be only created by newNode method of Graph class. 
 *
 * @par
 * Nodes have associated QDomElement for XML export support. The updateElement() routine should be called before 
 * export to get element in sync with node's properties.
 *
 * @sa Graph
 * @sa Edge
 * @sa Mark
 * @sa Nums
 */
class Node: public Marked, public Numbered, public PoolObj, public SListIface< Node>
{
public:
    /**
     * @brief Destructor.
     * Destructs the node. Operator delete shouldn't be called directly.
     * Use Graph::deleteNode for freeing memory and destruction
     */
    virtual ~Node();
    
    inline QDomElement elem() const;           /**< Return corresponding element */
    inline void setElement( QDomElement elem); /**< Set element                  */

    inline GraphUid id() const;  /**< Get node's unique ID           */
    inline Graph * graph() const;/**< Get node's corresponding graph */
    inline Node* nextNode();     /**< Next node in graph's list      */
    inline Node* prevNode();     /**< Prev node in graph's list      */
    
    /** Add edge to node in specified direction */
    inline void AddEdgeInDir( Edge *edge, GraphDir dir);
    
    inline void AddPred( Edge *edge); /**< Add predecessor edge */
    inline void AddSucc( Edge *edge); /**< Add successor edge   */
 
    /** Get first edge in given direction */
    inline Edge* firstEdgeInDir( GraphDir dir);
    
    inline Edge* firstSucc(); /**< Get first successor edge    */
    inline Edge* firstPred(); /**< Get first predecessor edge  */
     
    /** Deletion of edge in specified direction */
    void deleteEdgeInDir( GraphDir dir, Edge* edge);

    inline void deletePred( Edge* edge); /**< Delete predecessor edge */
    inline void deleteSucc( Edge* edge); /**< Delete successor edge   */
    
    virtual void debugPrint(); /**< Print node in DOT format to stdout */

    virtual void updateElement();                    /**< Update DOM element       */ 
    virtual void readFromElement( QDomElement elem); /**< Read properties from XML */

    /* Iterator types */
    typedef EdgeIterIface< SuccIterImpl> Succ;      /**< Iterator for successors       */
    typedef EdgeIterIface< PredIterImpl> Pred;      /**< Iterator for predecessors     */
    typedef EdgeIterIface< UnDirIterImpl> EdgeIter; /**< Undirected iterator for edges */
     
    inline Succ succsBegin(); /**< Create iterator for first succ       */
    inline Succ succsEnd();   /**< Create iterator pointing to succ end */
    
    inline Pred predsBegin(); /**< Create iterator for first succ       */
    inline Pred predsEnd();   /**< Create iterator pointing to succ end */

    inline EdgeIter edgesBegin(); /**< Create iterator for first succ       */
    inline EdgeIter edgesEnd();   /**< Create iterator pointing to succ end */

protected:
    /** We can't create nodes separately, do it through newNode method of graph */
    inline Node( Graph *_graph_p, GraphUid _id);
private:
	/** Graph class controls nodes */
    friend class Graph;	
    
    /** Detach this node from graph's node list */
    inline void detachFromGraph();

    /** Representation in document */
    QDomElement element;

    /* Connection with inclusive graph */
    GraphUid uid;   /**< Unique id        */
    Graph * graph_p;/**< Pointer to graph */

    /** First edges in graph's directions */
    Edge *first_edge[ GRAPH_DIRS_NUM];
};

#endif /* NODE_H */
