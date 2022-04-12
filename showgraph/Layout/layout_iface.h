/**
 * @file: layout_iface.h 
 * Layout interface
 *
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

#include <QList>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#else
#include <QtGui>
#endif

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
