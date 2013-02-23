/**
 * @file: layout_iface.h 
 * Layout interface
 *
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * @defgroup Layout 2D Graph layout
 * @ingroup Core
 * @page layout Graph layout
 * Drawing graph on a 2D plane is done via Sugiyama-like algorithm. 
 * Drawing is done in two steps:
 * - Ranking, wich defines vertical position of every node and inserts
 *   pseudo nodes on edges that span across multiple levels.
 * - Horizontal placement, wich uses barycenter heuristic to position a node close
 *   to the nodes connected with it on other levels
 */
#ifndef LAYOUT_IFACE_H
#define LAYOUT_IFACE_H

class AuxNode;
class AuxEdge;
class AuxGraph;
class Level;

#include <QtGui>
#include <QList>

/** Spacing between simple nodes */
const qreal NODE_NODE_MARGIN = 30;
/** Spacing between node and edge control */
const qreal NODE_CONTROL_MARGIN = 5;
/** Spacing between edge controls */
const qreal CONTROL_CONTROL_MARGIN = 5;
/** Edge control width */
const qreal EDGE_CONTROL_WIDTH = 5;
/** Edge control height */
const qreal EDGE_CONTROL_HEIGHT = 5;
/** Vertical spacing between ranks */
const qreal RANK_SPACING = 40;


/** Rank type and its undefined constant */
typedef unsigned int Rank;
/** Rank undefined value constant */
const Rank RANK_UNDEF = (Rank) (-1);

/**
 * Debug assert for layout library
 */
#if !defined(LAYOUT_ASSERTD)
#    define LAYOUT_ASSERTD(cond, what) ASSERT_XD(cond, "Layout", what)
#endif

#include "../Graph/graph_iface.h"
#include "aux_node.h"
#include "aux_edge.h"
#include "aux_graph.h"
#include "node_group.h"

#endif /** LAYOUT_IFACE_H */