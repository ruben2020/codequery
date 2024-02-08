/**
 * @file: graph_view.cpp
 * Graph View Widget implementation.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009 Boris Shurygin
 */
#include "gview_impl.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
#define WHEELEVENTYDELTA(x)           (x->angleDelta().y())
#else
#define WHEELEVENTYDELTA(x)           (x->delta())
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define QGV_MATRIX()       transform()
#define QGV_SETMATRIX(x)   setTransform(x)
#define QMATRIX            QTransform
#else
#define QGV_MATRIX()       matrix()
#define QGV_SETMATRIX(x)   setMatrix(x)
#define QMATRIX            QMatrix
#endif


/**
 * Compute scale parameter from user-controlled zoom factor
 */
inline qreal scaleVal( qreal zoom_scale)
{
    return qPow( qreal(2), zoom_scale / qreal(5));
}

/** Destructor */
GGraph::~GGraph()
{
    freeMarker( nodeTextIsShown);
    for ( GNode *node = firstNode();
          isNotNullP( node);
          )
    {
        GNode* next = node->nextNode();
        int ir_id = node->irId();
        deleteNode( node);
        node = next;
    }
    foreach ( GStyle *style, styles)
    {
        delete style;
    }
}

GNode*
GGraph::newNode()
{
    GNode* n = static_cast< GNode *>( AuxGraph::newNode());
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ)
{
    GEdge* e = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ));
    return e;
}

GNode*
GGraph::newNode( QDomElement e)
{
    GNode* n =  static_cast< GNode *>( AuxGraph::newNode( e));
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ, QDomElement e)
{
    GEdge* edge_p = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ, e));
    return edge_p;
}

void GGraph::selectOneNode( GNode* n)
{    
    emptySelection();
    selectNode( n);
    n->item()->highlight();
    n->item()->update();
}

/**
 * Change node's style
 */
void GGraph::setNodeStyle( GStyle *style)
{
    if ( sel_nodes.isEmpty())
        return;

    GNode* node = sel_nodes.first();
    node->setStyle( style);
    node->item()->adjustAssociates();
}

/**
 * Change edge's style
 */
void GGraph::setEdgeStyle( GStyle *style)
{
    if ( sel_edges.isEmpty())
        return;

    GEdge* edge = sel_edges.first();
    edge->setStyle( style);
    edge->item()->adjust();
}

void GGraph::showNodesText()
{
    foreach (GNode *n, sel_nodes)
    {
        view()->showNodeText( n);
    }
}

/**
 * Delete scheduled nodes
 */
void GGraph::deleteNodes()
{
    foreach (GNode *n, sel_nodes)
    {
        deleteNode( n);
    }
    sel_nodes.clear();
}

/**
 * Delete scheduled edges
 */
void GGraph::deleteEdges()
{
	foreach (GEdge *e, sel_edges)
	{
		deleteEdgeWithControls( e);
	}
    sel_edges.clear();
}
/**
 * Create self edge on selected node
 */
void GGraph::createSelfEdge()
{
    if ( !sel_nodes.isEmpty())
    {
        GNode *n = sel_nodes.first();
        newEdge( n, n);
    }
}

/** Create label on selected edge */
void GGraph::createEdgeLabel( QPointF pos)
{
    if ( !sel_edges.isEmpty())
    {
        sel_edges.first()->insertLabelNode( pos);
    }
}

void GGraph::showEdgePred()
{
    if ( !sel_edges.isEmpty())
    {
        if ( view()->isContext())
        {
            selectNode( sel_edges.first()->realPred());
            view()->findContext();
        } else
        {
            view()->focusOnNode( sel_edges.first()->realPred(), true);
        }
    }
}

void GGraph::showEdgeSucc()
{
    if ( !sel_edges.isEmpty())
    {
        if ( view()->isContext())
        {
            selectNode( sel_edges.first()->realSucc());
            view()->findContext();
        } else
        {
            view()->focusOnNode( sel_edges.first()->realSucc(), true);
        }
    }
}

void GGraph::clearNodesPriority()
{
    GNode* n;
    foreachNode( n, this)
    {
        n->setPriority( 0);
    }
}

/** Style edit finished */
void GraphView::styleEditFinished( int result)
{
    if ( isNotNullP( style_edit_info->node))
    {
        GNode *node = style_edit_info->node;
        GStyle *old_style = style_edit_info->old_style;
        GStyle *new_style = style_edit_info->new_style;

        if ( result == QDialog::Accepted)
        {
            if ( isNotNullP( old_style))
            {
                *(old_style) = *(new_style);
                node->setStyle( old_style);
                delete new_style;
            } else
            {
                QString name = QString("node %1 style").arg(node->id());
                new_style->setName( name);
                graph()->addStyle( name, new_style);
            }
        } else
        {
            node->setStyle( old_style);
            delete new_style;
        }
        node->item()->adjustAssociates();
    } else
    {
    
    }

    style_edit_info->dialog->disconnect();
    style_edit_info->dialog->deleteLater();
    delete style_edit_info;
}

void GGraph::showEditEdgeStyle()
{
    if ( sel_edges.isEmpty())
        return;
    
    StyleEdit dialog;
    GEdge* edge = sel_edges.first();
    
    GStyle* old_style = edge->style();
    GStyle* new_style;
    if ( isNullP( old_style))
    {
        QColor color( view()->palette().windowText().color());
        new_style = new GStyle();
        new_style->setPenColor( color);
        new_style->setPenWidth( 1);
    } else
    {
        new_style = new GStyle( *edge->style());
    }    
    dialog.setWindowTitle( "Edge style editor");
    dialog.setGStyle( new_style);
    edge->setStyle( new_style);
    connect( &dialog, SIGNAL( styleChanged( GStyle *)), view(), SLOT( setEdgeStyle( GStyle *)));
    if ( dialog.exec() == QDialog::Accepted)
    {
        if ( isNotNullP( old_style))
        {
            *(old_style) = *(new_style);
            edge->setStyle( old_style);
            delete new_style;
        } else
        {
            QString name = QString("edge %1 style").arg(edge->id());
            new_style->setName( name);
            styles[ name] = new_style;
        }
    } else
    {
        edge->setStyle( old_style);
        delete new_style;
    }
    edge->adjustStyles();
    edge->item()->adjust();
}

void GGraph::showEditNodeStyle()
{
    if ( sel_nodes.isEmpty())
        return;
    
    StyleEdit* dialog = new StyleEdit( NULL, true);
    GNode* node = sel_nodes.first();
    
    GStyle* old_style = node->style();
    GStyle* new_style;
    if ( isNullP( old_style))
    {
        QColor color( view()->palette().windowText().color());
        new_style = new GStyle();
        new_style->setPenColor( color);
        new_style->setPenWidth( 1);
    } else
    {
        new_style = new GStyle( *node->style());
    }   
    
    view()->setStyleEditInfo( new GraphView::StyleEditInfo( node, old_style, new_style, dialog));

    dialog->setWindowTitle( "Node style editor");
    dialog->setGStyle( new_style);
    node->setStyle( new_style);
    connect( dialog, SIGNAL( styleChanged( GStyle *)), view(), SLOT( setNodeStyle( GStyle *)));
    connect( dialog, SIGNAL( finished( int)), view(), SLOT( styleEditFinished( int)));
    dialog->exec();
}

void GGraph::showWholeGraph()
{
    assertd( !view()->isContext());
    emptySelection();
    GNode* n;
    foreachNode( n, this)
    {
        n->item()->setVisible( true);
        n->item()->setOpacityLevel( 1);
        n->setPriority( MAX_OPACITY);
        n->setForPlacement( true);
        n->setStable( false);
        n->item()->update();
    }
    doLayout();
}

/** Create label on selected edge */
void GGraph::findContext()
{
    if ( sel_nodes.isEmpty())
        return;
    
    QQueue< GNode *> border;
    Marker m = newMarker();
    foreach( GNode *n, sel_nodes)
    {
        n->mark( m);
        n->setPriority( GVIEW_MAX_PRIORITY);
        //n->setStable( true);
        border.enqueue( n);
    }
    for ( int i = 0; i < MAX_PLACE_LEN; i++)
    {
        int added = border.count();
        for ( int j = 0; j < added; j++)
        {
            GNode *n = border.dequeue(); 
            if ( !n->isPseudo())
            {
                selectNode( n);
                n->item()->highlight();
            }
            n->item()->update();
            GEdge *e;
            foreachPred( e, n)
            {
                GNode * pred = e->pred();
                
                if ( pred->mark( m))
                {
                    if ( i <= MAX_VISIBLE_LEN 
                         && pred->priority() < MAX_VISIBLE_LEN - i)
                    {
                        pred->setPriority( MAX_VISIBLE_LEN - i);
                    }/* else
                    {
                        pred->setPriority( 0);
                    }*/
                    border.enqueue( pred);
                }
            }
            foreachSucc( e, n)
            {
                GNode * succ = e->succ();
                if ( succ->mark( m))
                {
                    if ( i <= MAX_VISIBLE_LEN
                         && succ->priority() < MAX_VISIBLE_LEN - i)
                    {
                        succ->setPriority( MAX_VISIBLE_LEN - i);
                    }/* else
                    {
                        succ->setPriority( 0);
                    }*/
                    border.enqueue( succ);
                }
            }
        }
    }
    if ( view()->isContext())
    {
        GNode* n;
        foreachNode( n, this)
        {
            /*if ( n->item()->isVisible())
            {
                n->setStable();
            }*/
            if ( n->isMarked( m))
            {
                if ( !n->item()->isVisible())
                {
                    n->item()->setVisible( true);
                    n->item()->setOpacityLevel( 0);
                }
                n->setForPlacement( true);
            } else
            {
                if ( n->priority() < GVIEW_MAX_PRIORITY)
                {
                    n->setPriority( 0);
                    n->setForPlacement( false);
                }
                n->setStable( false);
            }
        }
        GEdge *e;
        foreachEdge( e, this)
        {
            if ( e->pred()->isPseudo() && e->succ()->isPseudo())
            {
                if ( e->realPred()->item()->isVisible()
                     && e->realSucc()->item()->isVisible())
                {
                    int priority = 
                        min<int>( e->realPred()->priority(), e->realSucc()->priority());
                    e->pred()->item()->setVisible( true);   
                    e->succ()->item()->setVisible( true);   
                    e->pred()->setForPlacement( true);
                    e->succ()->setForPlacement( true);
                    e->pred()->setPriority( priority);
                    e->succ()->setPriority( priority);
                }
            }
        }
        selectOneNode( sel_nodes.first());
    
        if ( view()->hasSmoothFocus())
        {
            view()->focusOnNode( sel_nodes.first(), true);
        }
        doLayout(); 
        foreachNode( n, this)
        {
            if ( n->item()->isVisible() && !n->item()->opacityLevel())
            {
                n->item()->setPos( n->modelX(), n->modelY());
            }
        }
        view()->startAnimationNodes();
    }
    freeMarker( m);
    
}

void GGraph::deleteEdgeWithControls( GEdge *edge)
{
	QList< GNode *> nodes;
	QList< GEdge *> edges;
    
    /** Check successor */
    GNode * succ = edge->succ();
    while ( succ->isEdgeControl() || succ->isEdgeLabel())
    {
        assertd( isNotNullP( succ->firstSucc()));
        nodes << succ;
        edges << succ->firstSucc();
		succ = succ->firstSucc()->succ();
    }
    GNode * pred = edge->pred(); 
    while ( pred->isEdgeControl() || pred->isEdgeLabel())
    {
        assertd( isNotNullP( pred->firstPred()));
        nodes << pred;
		edges << pred->firstPred();
        pred = pred->firstPred()->pred();
    }
    deleteEdge( edge);
	foreach ( GEdge *e, edges)
    {
        deleteEdge( e);
    }
    foreach ( GNode *n, nodes)
    {
        deleteNode( n);
    }
	
}

void GGraph::UpdatePlacement()
{
	QGraphicsScene *scene = view()->scene();
	
	/**
	 * Prevent BSP tree from being modified - significantly speeds up node positioning 
	 * For this purpose tree depth is set to one and indexing is turned off
	 */
	int depth = scene->bspTreeDepth();
    scene->setBspTreeDepth( 1);
    scene->setItemIndexMethod( QGraphicsScene::NoIndex);
    GNode *n;
	for ( n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        qreal x, y;
        
        if ( !n->isEdgeControl())
        {
            x = n->modelX() + ( n->item()->borderRect().width() - n->item()->textRect().width()) / 2;
            y = n->modelY() + ( n->item()->borderRect().height() - n->item()->textRect().height()) / 2;

            //QString str = QString("DFS num %1").arg(n->order());
            //n->item()->setPlainText(str);
        } else
        {
            x = n->modelX() + n->item()->borderRect().width() / 2;
            y = n->modelY() + n->item()->borderRect().height() / 2;
        }
        n->item()->setPos( x, y);
    }

    GEdge *e;
    
    foreachNode( n, this)
    {
        n->item()->show();
    }
    foreachEdge( e, this)
    {
        e->item()->show();
    }
	/** Restore indexing */
	scene->setItemIndexMethod( QGraphicsScene::BspTreeIndex);
    scene->setBspTreeDepth( depth); 
}
/**
 * Run layout procedure
 */
void GGraph::doLayout()
{
    if ( view()->isContext())
    {
        if ( rankingValid())
        {
            arrangeHorizontally();
        } else
        {
            /** Run layout algorithm */
	        AuxGraph::doLayout();
            UpdatePlacement();
        }
    } else
    {
        GNode *n;
        GEdge *e;
        
        foreachNode( n, this)
        {
            n->item()->hide();
        }
        foreachEdge( e, this)
        {
            e->item()->hide();
        }
        /** Run layout algorithm */
	    AuxGraph::doLayoutConcurrent();
        //AuxGraph::doLayout();
        //layoutPostProcess();
    }
}

/**
 * Run layout procedure in single thread
 */
void GGraph::doLayoutSingle()
{
    if ( view()->isContext())
    {
        if ( rankingValid())
        {
            arrangeHorizontally();
        } else
        {
            /** Run layout algorithm */
	        AuxGraph::doLayout();
            UpdatePlacement();
        }
    } else
    {
        GNode *n;
        GEdge *e;
        
        foreachNode( n, this)
        {
            n->item()->hide();
        }
        foreachEdge( e, this)
        {
            e->item()->hide();
        }
        /** Run layout algorithm */
	    AuxGraph::doLayout();
        layoutPostProcess();
    }
}

void GGraph::layoutPostProcess()
{
    UpdatePlacement();
    
    /** Center view on root node */
    GNode *root = static_cast<GNode*>( rootNode());
    if ( isNotNullP( root))
    {
        view_p->centerOn( root->item());
    }
}

/** Constructor */
GraphView::GraphView(): 
    curr_pos(),
    createEdge( false),
    graph_p( 0),
	zoom_scale( 0),
    view_history( new GraphViewHistory),
    timer_id( 0),
    node_animation_timer( 0),
    smooth_focus( false),
    editable( false),
    view_mode( WHOLE_GRAPH_VIEW),
    style_edit_info( NULL),
    helper( new EdgeHelper)
{
    QGraphicsScene *scene = new QGraphicsScene( this);
    //scene->setItemIndexMethod( QGraphicsScene::NoIndex);
    //scene->setSceneRect(0, 0, 10000, 10000);
    setScene( scene);
    //setCacheMode( CacheBackground);
    setViewportUpdateMode( SmartViewportUpdate);
    setRenderHint( QPainter::Antialiasing);
    setTransformationAnchor( AnchorViewCenter);
    setResizeAnchor( AnchorViewCenter);
    setMinimumSize( 200, 200);
    setWindowTitle( tr("ShowGraph"));
    setDragMode( ScrollHandDrag);
    //setDragMode( RubberBandDrag);
    tmpSrc = NULL;
    search_node = NULL;
	createActions();
	createMenus();
	show_menus = true;
	setAcceptDrops( false);
    scene->addItem( helper);
}

/** Destructor */
GraphView::~GraphView()
{
    scene()->setItemIndexMethod( QGraphicsScene::NoIndex);
    delete graph_p;
    delete view_history;
    delete helper;
}

void GraphView::startAnimationNodes()
{
    if ( !node_animation_timer)
         node_animation_timer = startTimer(1000/25);
}

void 
GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void 
GraphView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QGraphicsView::mouseDoubleClickEvent( ev);   
    if( ev->button() & Qt::LeftButton)
    {
        QPoint p = ev->pos();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        if ( isEditable() && !scene()->itemAt( mapToScene( ev->pos()), QTransform()))
#else
        if ( isEditable() && !scene()->itemAt( mapToScene( ev->pos())))
#endif
        {
            GNode* node = graph()->newNode();
            QString text = QString("Node %1").arg( node->id());
            node->item()->setPlainText( text);
            node->item()->setPos( mapToScene( p));
        }
    } else if( isEditable() 
               && ev->button() & Qt::RightButton)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()), QTransform());
#else
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()));
#endif
        if ( isNotNullP( node) && qgraphicsitem_cast<NodeItem *>( node))
        {
            graph()->emptySelection();
            graph()->setNodeInFocus( NULL);
            graph()->deleteNode( qgraphicsitem_cast<NodeItem *>( node)->node());
        }
    }
}

void
GraphView::mousePressEvent(QMouseEvent *ev)
{
    if( timer_id)
    {
        killTimer( timer_id);
        timer_id = 0;
    }
    QGraphicsView::mousePressEvent( ev);
}

/** Enable/disable edition */
void GraphView::toggleEdition( bool e)
{
    setEditable( e);
}

/** Insert node in center of view */
void GraphView::insertNodeOnCenter()
{

}

/** Run layout */
void GraphView::runLayout()
{
    graph()->doLayout();
}

void
GraphView::contextMenuEvent( QContextMenuEvent * e)
{

}

void
GraphView::mouseReleaseEvent( QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        if ( createEdge)
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()), QTransform());
#else
            QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()));
#endif
            if ( isNotNullP( item) && qgraphicsitem_cast<NodeItem *>(item))
            {
                if ( tmpSrc != qgraphicsitem_cast<NodeItem *>(item)->node())
                {
                    GNode* dst_node = qgraphicsitem_cast<NodeItem *>(item)->node();
                    if ( tmpSrc->isSimple() && dst_node->isSimple())
                    {   
                        graph()->newEdge( tmpSrc, dst_node);
                        show_menus = false;
                    }
				}
            }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        } else if ( !scene()->itemAt( mapToScene( ev->pos()), QTransform()))
#else
        } else if ( !scene()->itemAt( mapToScene( ev->pos())))
#endif
        {
            QMenu *menu = new QMenu( tr( "&View Menu"));
            menu->addAction( editableSwitchAct);
            menu->addAction( runLayoutAct);
            menu->exec( mapToGlobal( ev->pos()));
            delete menu;
        }
    }
    helper->hide();
    helper->reset();
    QGraphicsView::mouseReleaseEvent(ev);
	createEdge = false;
	show_menus = true;
	tmpSrc = NULL;
}

void
GraphView::mouseMoveEvent(QMouseEvent *ev)
{
    if ( createEdge)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()), QTransform());
#else
        QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()));
#endif
        if ( isNotNullP( item))
        {
            NodeItem *node_item = qgraphicsitem_cast<NodeItem *>(item);
            if ( isNotNullP( node_item)
                 && node_item->node()->isSimple())
            {   
                if ( areNotEqP( node_item->node(), tmpSrc))
                    helper->switchToRegular();
                helper->setDst( node_item);
            } else
            {
                helper->switchToRegular();
                helper->setDstP( helper->mapFromScene( mapToScene( ev->pos())));
                helper->setDst( NULL);
            }
        } else
        {
            helper->switchToRegular();
            helper->setDstP( helper->mapFromScene( mapToScene( ev->pos())));
            helper->setDst( NULL);
        }
        helper->adjust();
    }
    QGraphicsView::mouseMoveEvent(ev);
}

void 
GraphView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent( event);
    if ( !event->isAccepted())
    {
        if ( event->key() == Qt::Key_Plus)
        {
            zoomIn();
        } else if ( event->key() == Qt::Key_Minus)
        {
            zoomOut();
        }
    }
}

/**
 * Actions for wheel event
 */
void GraphView::wheelEvent(QWheelEvent *event)
{
    zoom_scale += WHEELEVENTYDELTA(event) / 100;
	updateMatrix();
}

void GraphView::zoomIn()
{
    zoom_scale++;
    updateMatrix();
}

void GraphView::zoomOut()
{
	zoom_scale--;
    updateMatrix();
}

void GraphView::zoomOrig()
{
    zoom_scale = 0;
    ensureVisible( 0,0,0,0);
    checkDelItems();
    updateMatrix();
}

void GraphView::updateMatrix()
{
     qreal scale_val = scaleVal( zoom_scale); 
     QMATRIX scale;
     qreal prev_scale = QGV_MATRIX().m11();
     qreal scale_ratio = scale_val / prev_scale; 
     scale.scale( scale_val, scale_val);
     GNode * focus = graph()->nodeInFocus();
     if ( isNotNullP( focus))
     {
         QPointF item_center = focus->item()->mapToScene( focus->item()->boundingRect())
                               .boundingRect()
                               .center();
         QPointF old_center = mapToScene( viewport()->rect())
                             .boundingRect()
                             .center();

         QPointF item_in_view = item_center - old_center;
         QGV_SETMATRIX( scale); // scale
         QPointF new_center = old_center + item_in_view * ( scale_ratio - 1) / scale_ratio;
         centerOn( new_center); // adjust to keep focus point in place
     } else
     {
         QGV_SETMATRIX( scale);
     }
     
}

void
GraphView::deleteItems()
{
    int depth = scene()->bspTreeDepth();
    scene()->setBspTreeDepth( 1);
    scene()->setItemIndexMethod( QGraphicsScene::NoIndex);
    foreach ( NodeItem* item, del_node_items)
    {
        del_node_items.removeAll( item);  
        delete item;
    }
    foreach ( EdgeItem* item, del_edge_items)
    {
        del_edge_items.removeAll( item);  
        delete item;  
    }
    scene()->setItemIndexMethod( QGraphicsScene::BspTreeIndex);
    scene()->setBspTreeDepth( depth);
}

void GraphView::deleteSelected()
{
    graph()->deleteNodes();
	graph()->deleteEdges();
}

void GraphView::createSESelected()
{
    graph()->createSelfEdge();
}

void GraphView::createEdgeLabel()
{
    graph()->createEdgeLabel( curr_pos);
}

void GraphView::showEdgePred()
{
    graph()->showEdgePred();
}

void GraphView::showEdgeSucc()
{
    graph()->showEdgeSucc();
}

void GraphView::findContext()
{
    graph()->findContext();
}

/** Show text of the clicked node */
void GraphView::showSelectedNodesText()
{
    graph()->showNodesText();
}

/** Show style editor for node */
void GraphView::showEditNodeStyle()
{
    graph()->showEditNodeStyle();
}
/** Show style editor for edge */
void GraphView::showEditEdgeStyle()
{
    graph()->showEditEdgeStyle();
}

void GraphView::createActions()
{
    QString system = QLatin1String("win");
#ifdef Q_OS_MAC
    system = QLatin1String("mac");
#endif

    deleteItemAct = new QAction(tr("&Delete"), this);
    deleteItemAct->setShortcut(tr("Ctrl+D"));
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT( deleteSelected()));

    createSelfEdgeAct = new QAction(tr("&Create self-edge"), this);
    connect( createSelfEdgeAct, SIGNAL(triggered()), this, SLOT( createSESelected()));

    createEdgeLabelAct = new QAction(tr("&Create label"), this);
    connect( createEdgeLabelAct, SIGNAL(triggered()), this, SLOT( createEdgeLabel()));

    findContextAct = new QAction(tr("&Highlight context"), this);
    connect( findContextAct, SIGNAL(triggered()), this, SLOT( findContext()));
    
    showTextAct = new QAction(tr("&Show text"), this);
    connect( showTextAct, SIGNAL(triggered()), this, SLOT( showSelectedNodesText()));

    showPredAct = new QAction(tr("Pred"), this);
    connect( showPredAct, SIGNAL(triggered()), this, SLOT( showEdgePred()));

    showSuccAct = new QAction(tr("Succ"), this);
    connect( showSuccAct, SIGNAL(triggered()), this, SLOT( showEdgeSucc()));

    editableSwitchAct = 
        new QAction( QIcon( QString::fromUtf8(":/images/%1/Edit/Edit.ico").arg( system)),
                     tr("Editable"), this);
    editableSwitchAct->setCheckable( true);
    editableSwitchAct->setChecked( false);
    connect( editableSwitchAct, SIGNAL( toggled( bool)), this, SLOT( toggleEdition( bool)));
    
    insertNodeAct = new QAction(tr("Insert node"), this);
    
    showEditNodeStyleAct = new QAction(tr("Change style"), this);
    connect( showEditNodeStyleAct, SIGNAL( triggered()), this, SLOT(showEditNodeStyle()));

    showEditEdgeStyleAct = new QAction(tr("Change style"), this);
    connect( showEditEdgeStyleAct, SIGNAL( triggered()), this, SLOT(showEditEdgeStyle()));

    runLayoutAct = 
        new QAction( QIcon( QString::fromUtf8(":/images/%1/Synchronize/Synchronize.ico").arg( system)),
                     tr("&Run Layout"), this);
    connect( runLayoutAct, SIGNAL(triggered()), this, SLOT( runLayout()));
}

void GraphView::createMenus()
{
    nodeItemMenu = new QMenu( tr( "&Node Item"));
    nodeItemMenu->addAction( deleteItemAct);
    nodeItemMenu->addAction( createSelfEdgeAct);

	edgeItemMenu = new QMenu( tr( "&Edge Item"));
    edgeItemMenu->addAction( deleteItemAct);
}

QMenu* GraphView::createMenuForNode( GNode *n)
{
    QMenu* menu = new QMenu( tr( "&Node"));
    menu->addAction( deleteItemAct);
    deleteItemAct->setEnabled( isEditable());
    if ( !n->isPseudo())
    {
        menu->addAction( findContextAct);
        menu->addAction( showTextAct);
        menu->addSeparator();
        menu->addAction( createSelfEdgeAct);
        createSelfEdgeAct->setEnabled( isEditable());
        menu->addAction( showEditNodeStyleAct);
        showEditNodeStyleAct->setEnabled( isEditable());
    }
    return menu;
}

QMenu* GraphView::createMenuForEdge( GEdge *e)
{
    QMenu* menu = new QMenu( tr( "&Edge"));
    menu->addAction( showPredAct);
    menu->addAction( showSuccAct);
    menu->addSeparator();
    menu->addAction( deleteItemAct);
    deleteItemAct->setEnabled( isEditable());
    menu->addAction( createEdgeLabelAct);
    createEdgeLabelAct->setEnabled( isEditable());
    menu->addAction( showEditEdgeStyleAct);
    showEditEdgeStyleAct->setEnabled( isEditable());
    return menu;
}

void GraphView::dragEnterEvent(QDragEnterEvent *event)
{
	/*if ( event->mimeData()->hasUrls())
		event->acceptProposedAction();*/
}

void GraphView::dropEvent(QDropEvent *event)
{
	/*const QMimeData *mimeData = event->mimeData();

	if ( mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		QString text;

		for ( int i = 0; i < urlList.size(); ++i)
		{
			QString url = urlList.at(i).path();

		}
    }

	event->acceptProposedAction();*/
}
 
void GraphView::dragMoveEvent( QDragMoveEvent *event)
{
	//event->acceptProposedAction();
}

void GraphView::advanceNodes()
{
    GNode *n;
    bool advanced = false;
    foreachNode( n, graph())
    {
        if ( n->item()->advance())
            advanced = true;
    }
    if ( !advanced)
    {
        killTimer( node_animation_timer);
        node_animation_timer = 0;
    }
}

void GraphView::advanceView()
{
    GNode *target = graph()->nodeInFocus(); 
    qreal STEP_LEN = 10 / scaleVal( zoom_scale);
    const qreal STEP_SCALE = 0.2;
    if ( isNotNullP( target))
    {
        QRectF item_rect = target->item()->mapToScene( target->item()->boundingRect()).boundingRect();
        QRectF view_rect = mapToScene( viewport()->rect())
                           .boundingRect();
        
        QLineF line( view_rect.center(), item_rect.center());
        
        qreal dist = line.length();
        QPointF displacement( STEP_LEN * line.dx() / dist, STEP_LEN * line.dy() / dist);
        if ( dist > STEP_LEN /2)
            centerOn( view_rect.center() + displacement);
        
        QRectF new_view_rect = mapToScene( viewport()->rect()).boundingRect();
        QLineF step( new_view_rect.center(), view_rect.center());
        qreal len = step.length();
        
        if ( zoom_out_done)
        {   
            if ( dist < STEP_LEN || len < STEP_LEN /2)
            {
                zoom_scale+=STEP_SCALE;
                updateMatrix();
                if ( abs<qreal>( zoom_scale - preferred_zoom) <= STEP_SCALE * 2)
                {
                    zoom_scale = preferred_zoom;
                    updateMatrix();
                    killTimer( timer_id);
                    timer_id = 0;
                    centerOn( target->item());
                }
            }
        }
        if ( ! zoom_out_done
             && ( abs<qreal>( line.dx()) > view_rect.width()
                  || abs<qreal>( line.dy()) > view_rect.height()))
        {
            zoom_scale-=STEP_SCALE;
            QMATRIX scale;
            scale.scale( scaleVal( zoom_scale), scaleVal( zoom_scale));
            QGV_SETMATRIX( scale);
        } else
        {
            zoom_out_done = true;
        }
    }
}

void GraphView::timerEvent( QTimerEvent *event)
{
    if ( !event->timerId())
        return;
    if ( event->timerId() == timer_id)
    {
        advanceView();
    }
    if ( event->timerId() == node_animation_timer)
    {
        advanceNodes();
    }
}
void GraphView::clearSearch()
{
    search_node = NULL;
}

void GraphView::focusOnNode( GNode *n, bool gen_event)
{
    graph()->selectOneNode( n);
    
    if ( smooth_focus)
    {
        if (!timer_id)
             timer_id = startTimer(1000/25);
        preferred_zoom = zoom_scale;
        zoom_out_done = false;
    } else
    {
        centerOn( n->item());
    }
    if ( gen_event && !n->isNodeInFocus())
        view_history->focusEvent( n);
    graph()->setNodeInFocus( n);
}

GNode *
GraphView::findNextNodeWithText( QString &findStr,
                                 QTextDocument::FindFlags flags)
{
	GNode *n;
	
    if ( isNullP( search_node))
    {
        foreachNode( n, graph())
        {
            if ( isNullP( n->nextNode()))
                break;
        }
    } else
    {
        n = search_node->prevNode();
    }

    while ( isNotNullP( n))
    {
		QTextDocument *doc = n->doc();
		if ( isNotNullP( doc) && !doc->find( findStr, 0, flags).isNull())
        {
            search_node = n;        
            break;
        }
        n = n->prevNode();
	}
	if ( isNotNullP( n))
	{
		if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
		return n;
	} else
	{
		search_node = NULL;        
        return NULL;
	}
}

GNode *
GraphView::findPrevNodeWithText( QString &findStr,
                                 QTextDocument::FindFlags flags)
{
	GNode *n;
	
    flags ^= QTextDocument::FindBackward; // Unset backward search flag
    for ( n = isNullP( search_node) ? graph()->firstNode() : search_node->nextNode();
          isNotNullP( n);
          n = n->nextNode())
    {
		QTextDocument *doc = n->doc();
		if ( isNotNullP( doc) && !doc->find( findStr, 0, flags).isNull())
        {
            search_node = n;        
            break;
        }
	}
	if ( isNotNullP( n))
	{
		if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
		return n;
	} else
	{
		search_node = NULL;        
        return NULL;
	}
}
/**
 * Find node by its ID from dump
 */
GNode* GraphView::findNodeById( int id)
{
	GNode *n;
	foreachNode( n, graph())
	{
        if ( !n->isEdgeControl()
             && !n->isEdgeLabel()
             && n->irId() == id)
			break;
	}
	if ( isNotNullP( n))
	{
        if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
		return n;
	} else
	{
		return NULL;
	}
}

/**
 * Find node by its label
 */
GNode* GraphView::findNodeByLabel( QString label)
{
	GNode *n;
	foreachNode( n, graph())
	{
        if ( !n->isEdgeControl()
             && !n->isEdgeLabel())
        {
            QString node_label = n->item()->toPlainText();        
            if ( node_label == label)
                 break;
        }
	}
	if ( isNotNullP( n))
	{
        if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
		return n;
	} else
	{
		return NULL;
	}
}

/** Repeat navigation event */
void GraphView::replayNavigationEvent( NavEvent *ev)
{
    assertd( isNotNullP( ev));
    
    if ( ev->isFocus())
    {
        assertd( isNotNullP( ev->node()));
        if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( ev->node());
            findContext();
        }
        focusOnNode( ev->node(), false); 
    }
}

void GraphView::toggleSmoothFocus( bool smooth)
{
    smooth_focus = smooth;
}

void GraphView::toggleViewMode( bool context)
{
    if ( context && CONTEXT_VIEW != view_mode)
    {
        view_mode = CONTEXT_VIEW;
        graph()->clearNodesPriority();
    } else if ( !context && WHOLE_GRAPH_VIEW != view_mode)
    {
        view_mode = WHOLE_GRAPH_VIEW;
        graph()->showWholeGraph();
    }
}

/** Navigate backward */
void GraphView::navPrev()
{
    NavEvent * ev = view_history->prev();
    if ( isNotNullP( ev))
        replayNavigationEvent( ev);
}

/** Navigate forward */
void GraphView::navNext()
{
    NavEvent * ev = view_history->next();
    if ( isNotNullP( ev))
        replayNavigationEvent( ev);
}
/** Erase node from history */
void GraphViewHistory::eraseNode( GNode *n)
{
    it = events.begin();
    while ( it != events.end())
    {
        NavEvent *ev = *it;
        if ( ev->isFocus() && areEqP( ev->node(), n))
        {
            it = events.erase( it);
        } else
        {
            it++;
        }
    }
}
/** Get last event */
NavEvent *GraphViewHistory::last()
{
    if ( events.count() > 0)
    {
        it = events.end();
        it--;
        return *it;
    }
    return NULL;
}
/** Get prev event */
NavEvent *GraphViewHistory::prev()
{
    if ( it != events.begin())
    {
        it--;
        return *it;
    }
    return NULL;
}
/** Get next event */
NavEvent *GraphViewHistory::next()
{
    if ( it != events.end())
        it++;
    if ( it != events.end())
    {
        return *it;
    }
    return NULL;
}
/** New event */
void GraphViewHistory::viewEvent( NavEventType t, GNode *n)
{
    NavEvent *ev = new NavEvent( t, n);
    if ( it != events.end())
        it++;
    if ( it != events.end())
        events.erase( it, events.end());
    events.push_back( ev);
    it = events.end();
    it--;
}

/**
 * Implementation of XML writing
 */
void 
GGraph::writeToXML( QString filename)
{
    QFile file( filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        assertd( 0);
        return;
    }
    
    QList< QDomElement> elements;

    foreach ( GStyle *style, styles)
    {
        if ( style->numItems() != 1)
        {
            QDomElement e = createElement( "style");
            elements.push_back( e);
            style->writeElement( e);
            //QDomNode e2 = graph()->documentElement().removeChild( e);
            //assertd( !e2.isNull());
            QDomNode n = documentElement().insertBefore( e, documentElement().firstChild());
            ASSERTD( !n.isNull());
        }
    }

    /** Update element for each node */
    for ( GNode *n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->updateElement();
    }

    /** Update element for each edge */
    for ( GEdge *e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->updateElement();
    }

    QTextStream out( &file);
    save(out, IndentSize);

    foreach ( QDomElement el, elements)
    {
        QDomNode n = documentElement().removeChild( el);
        ASSERTD( !n.isNull());
    }
}
/**
 * Build graph from XML description
 */
void
GGraph::readFromXML( QString txt)
{

    if ( !setContent( txt))
    {
        throw GGraphError( "Not a good-formated xml file");
    }

    /**
     * Read nodes and create them
     */
    QDomElement docElem = documentElement();

    QHash< GraphUid, GNode *> n_hash;
    
    for ( QDomNode n = docElem.firstChild();
          !n.isNull();
          )
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        QString error_msg = QString("in line %1: ").arg( n.lineNumber());

        //Not an element, proceed to next node
        if ( e.isNull())
            continue;
        
        /** Parse element and its attributes */
        //Node
        if ( e.tagName() == QString( "node"))
        {
            GNode *node = newNode( e);
            node->readFromElement( e);
            n_hash[ e.attribute( "id").toLongLong()] = node;

            if ( e.hasAttribute("style"))
            {
                if ( styles.find( e.attribute("style")) == styles.end())
                    throw GGraphError( error_msg.append("unknown style"));
                node->setStyle( styles[ e.attribute("style")]);
            } else
            {
                // Try to parse style-definining fields
                GStyle *style = new GStyle( e); // possible exception GGraphError
                if ( style->isDefault())
                {
                    delete style;
                } else
                {
                    QString name("node %1 style");
                    name.arg( node->id());
                    style->setName( name);
                    styles[ name] = style;
                    node->setStyle( style);
                }
            }
        }
        if ( e.tagName() == QString( "style"))
        {
            GStyle *style = new GStyle( e); // possible exception GGraphError
            styles[ e.attribute( "name")] = style;
            n = n.nextSibling();
            documentElement().removeChild( e);
            continue;
        }
        n = n.nextSibling();
    }
    
    for ( QDomNode n = docElem.firstChild();
          !n.isNull();
          n = n.nextSibling())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        QString error_msg = QString("in line %1: ").arg( n.lineNumber());

        /** Cannot convert node to element or this element is not a edge description */
        if ( e.isNull() 
             || !( e.tagName() == QString( "edge")))
             continue;
       
        if ( !e.hasAttribute("source"))
                throw GGraphError( error_msg.append("malformed edge description: no source"));

        if ( !e.hasAttribute("target"))
            throw GGraphError( error_msg.append("malformed edge description: no target"));

        bool ok = false;
        GraphUid pred_id = e.attribute( "source").toLongLong( &ok);
        if ( !ok)
            throw GGraphError( error_msg.append("malformed edge description: wrong source id"));

        GraphUid succ_id = e.attribute( "target").toLongLong( &ok);
        if ( !ok)
            throw GGraphError( error_msg.append("malformed edge description: wrong target id"));

        if ( n_hash.find( pred_id) == n_hash.end())
            throw GGraphError( error_msg
                               .append("malformed edge description: predecessor node missing in graph"));
        if ( n_hash.find( succ_id) == n_hash.end())
            throw GGraphError( error_msg
                               .append("malformed edge description: successor node missing in graph"));

        GNode *pred = n_hash[ pred_id];
        GNode *succ = n_hash[ succ_id];
        GEdge *edge = newEdge( pred, succ, e);
        edge->readFromElement( e);

        if ( e.hasAttribute("style"))
        {
            if ( styles.find( e.attribute("style")) == styles.end())
                throw GGraphError( error_msg.append("unknown style"));
            edge->setStyle( styles[ e.attribute("style")]);
        } else
        {
            // Try to parse style-definining fields
            GStyle *style = new GStyle( e); // possible exception GGraphError
            if ( style->isDefault())
            {
                delete style;
            } else
            {
                QString name("edge %1 style");
                name.arg( edge->id());
                style->setName( name);
                styles[ name] = style;
                edge->setStyle( style);
            }
        }
    }
}

/**
 * Change node's style
 */
void GraphView::setNodeStyle( GStyle *style)
{
    graph()->setNodeStyle( style);
}

/**
 * Change edge's style
 */
void GraphView::setEdgeStyle( GStyle *style)
{
    graph()->setEdgeStyle( style);
}
