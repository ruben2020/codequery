/**
 * @file: predecls.h
 * Predeclarations for interface of Graph library
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