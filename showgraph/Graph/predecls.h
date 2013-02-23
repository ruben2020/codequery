/**
 * @file: predecls.h
 * Predeclarations for interface of Graph library
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <QDomDocument>
#include <QtGlobal>
#include <QtGui>
#include <QList>
#include <QLinkedList>
#include <QVector>
#include <list>
using namespace std;

#include "../Utils/utils_iface.h"

/* namespaces import */
using namespace Utils;
using namespace Mem;

/**
 * Debug assert for graph library
 * @ingroup GraphBase
 */
#if !defined(GRAPH_ASSERTD)
#    define GRAPH_ASSERTD(cond, what) ASSERT_XD(cond, "Graph", what)
#endif

/**
 * Directions type in graph
 * @ingroup GraphBase
 */
enum GraphDir
{
    /** Upward direction */
	GRAPH_DIR_UP = 0,
	/** Downward direction */
    GRAPH_DIR_DOWN = 1,
	/** Number of directions in graph */
    GRAPH_DIRS_NUM = 2
};

/**
 * Return direction that is reverse to given one
 * @ingroup GraphBase
 */
inline GraphDir
RevDir( GraphDir dir)
{
    GRAPH_ASSERTD( GRAPH_DIRS_NUM == 2, "Graph implementation is suited for two directions only");
#ifdef DIR_INVERTION_LONG_VERSION
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP; 
#else
    return (GraphDir)!dir;
#endif
}

/**
 * Number type used for numbering nodes and edges in graph
 * @ingroup GraphBase
 */
typedef quint32 GraphNum;
/**
 * Node/edge ID type
 * @ingroup GraphBase
 */
typedef quint64 GraphUid;

/** 
 * Maximum number of nodes
 * @ingroup GraphBase
 */
const GraphNum GRAPH_MAX_NODE_NUM = ( GraphNum)( -1);
/**
 * Maximum number of edges
 * @ingroup GraphBase
 */
const GraphNum GRAPH_MAX_EDGE_NUM = ( GraphNum)( -1);

/**
 * Indentation for XML writing
 * @ingroup GraphBase
 */
const int IndentSize = 4;

/* Predeclarations of graph-related classes */
class Graph;
class Node;
class Edge;

#include "marker.h"
#include "num.h"

#endif 