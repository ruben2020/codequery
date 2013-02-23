/**
 * @file: graph_iface.h
 * Interface of Graph library
 *
 * @defgroup GraphBase Graph
 * @ingroup Core
 *
 * @brief Representation of graphs
 * 
 * Graph representation is implemented through 3 tightly connected classes Graph, Node and Edge.
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_IFACE_H
#define GRAPH_IFACE_H
#include "predecls.h"

#include "edge.h"
#include "node.h"
#include "graph.h"
#include "agraph.h"

/* Edges traversal implementation */
#define ITERATE_NODE_EDGES(node, edge, dir) for ( edge = node->first##dir();\
										          isNotNullP( edge);\
								                  edge = edge->next##dir())

/* Succs traverse implementation */
#define ITERATE_NODE_SUCCS(edge, node) for ( edge = node->firstSucc();\
										     isNotNullP( edge);\
								             edge = edge->nextSucc())

/* Preds traverse implementation */
#define ITERATE_NODE_PREDS(edge, node) for ( edge = node->firstPred();\
									         isNotNullP( edge);\
								             edge = edge->nextPred())

/**
 * Convenience macro for traversing edges in given direction
 * @ingroup GraphBase
 * @param node A node that we use to get first edge
 * @param edge An object of Edge type or subclass of Edge which is the loop variable
 * @param dir 'Succ' or 'Pred'
 */
#ifndef ForEdges
#  define ForEdges(node, edge, dir) ITERATE_NODE_EDGES(node, edge, dir)
#endif

/**
 * Convenience macro for traversing successors of node
 * @ingroup GraphBase
 * @param edge An object of Edge type or subclass of Edge which is the loop variable
 * @param node A node that we use to get first edge
 */
#ifndef foreachSucc
#  define foreachSucc(edge, node) ITERATE_NODE_SUCCS(edge, node)
#endif

/**
 * Convenience macro for traversing predecessors of node
 * @ingroup GraphBase
 * @param edge An object of Edge type or subclass of Edge which is the loop variable
 * @param node A node that we use to get first edge
 */
#ifndef foreachPred
#  define foreachPred(edge, node) ITERATE_NODE_PREDS(edge, node)
#endif

/* Graph's edges traverse implementation */
#define ITERATE_GRAPH_EDGES(edge, graph) for ( edge = graph->firstEdge();\
									           isNotNullP( edge);\
								               edge = edge->nextEdge())
/* Graph's nodes traverse implementation */
#define ITERATE_GRAPH_NODES(node, graph) for ( node = graph->firstNode();\
									           isNotNullP( node);\
								               node = node->nextNode())

/**
 * Convenience macro for traversing edges in graph
 * @ingroup GraphBase
 * @param edge An object of Edge type or subclass of Edge which is the loop variable
 * @param graph Graph of interest
 */
#ifndef foreachEdge
#  define foreachEdge(edge, graph) ITERATE_GRAPH_EDGES(edge, graph)
#endif

/**
 * Convenience macro for traversing nodes in graph
 * @ingroup GraphBase
 * @param node An object of Node type or subclass of Node which is the loop variable
 * @param graph Graph of interest
 */
#ifndef foreachNode
#  define foreachNode(node, graph) ITERATE_GRAPH_NODES(node, graph)
#endif

/* Implementation of inline functinality */
#include "edge_inline.h"
#include "node_inline.h"
#include "graph_inline.h"

#endif