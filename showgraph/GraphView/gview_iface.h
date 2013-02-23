/**
 * @file: gview_iface.h 
 * Graph view classes interface
 * @defgroup GUI Graphical User Interface 
 * @author Boris Shurygin
 *
 * Graphical part of ShowGraph is arranged around MainWindow wich has 
 * GraphView as a center widget.
 */
/**
 * Graph view implementation.
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef GVIEW_IFACE_H
#define GVIEW_IFACE_H

/**
 * Debug assert for graph library
 */
#if !defined(GVIEW_ASSERTD)
#    define GVIEW_ASSERTD(cond, what) ASSERT_XD(cond, "GUI", what)
#endif

/** Predeclarations */
class GGraph;
class GNode;
class GEdge;
class NodeItem;
class EdgeItem;
class GraphView;
class StyleEdit;

#include <QtGui>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

#include "../Graph/graph_iface.h"
#include "../Layout/layout_iface.h"

/**
 * Subclasses of QGraphicsItem types
 */
enum { 
    TypeNode = QGraphicsItem::UserType + 1, /** Node item */
    TypeEdge = QGraphicsItem::UserType + 2, /** Edge item */
    TypeEdgeControl = QGraphicsItem::UserType + 3,/** EdgeControl item */
    TypeEdgeHelper = QGraphicsItem::UserType + 4 /** Edge helper item */
};

/** Max opacity level for items in graph view */
const qreal MAX_OPACITY = 6;

/**
 * Exception type for graph errors
 * @ingroup GUIGraph
 */
class GGraphError
{
public:
    /** No message constructor */
    GGraphError(){};
    /** Message-based constructor*/
    GGraphError( QString str): msg_priv( str){};
    /** Get message */
    inline QString message()
    {
        return msg_priv;
    }
private:
    /** Error message */
    QString msg_priv;
};

#include "navigation.h"
#include "visible_edge.h"
#include "gstyle.h"
#include "edge_item.h"
#include "node_item.h"
#include "edge_helper.h"
#include "graph_view.h"
#include "style_edit.h"

#endif /* GVIEW_IFACE_H */