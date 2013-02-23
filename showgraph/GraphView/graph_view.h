/**
 * @file: graph_view.h 
 * Graph View class definition.
 * @defgroup GUIGraph Graph Visualization System
 * @ingroup GUI
 */
/* GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
/**
 * @page iface Graph editor interface
 * Graph editor.
 * 
 * Here's list of conventional editing tasks and how they are performed with Showgraph
 * -# Create new node - double click on free space
 * -# Create edge - draw edge holding down right mouse button
 * -# Delete node/edge - use delete option from context menu
 * -# Enter text in node - double click on node to enable its text editor
 * -# Move node - press left mouse button on it and drag
 * -# Create edge control point - double click on edge
 * -# Zoom view - press '-' and '+' or use mouse wheel
 * -# Invoke auto layout - select 'Run Layout' from View menu or simply press F5 
 */
#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "gview_impl.h"

/**
 * Navigation event types
 * @ingroup GUIGraph
 */
enum NavEventType
{
    /** Focus on node event */
    NAV_EVENT_NODE_FOCUS,
    /** Number of event types */
    NAV_EVENT_TYPES_NUM
};

/**
 * Navigation event
 * @ingroup GUIGraph
 */
class NavEvent
{
    /** data associated with event */
    union NavEventData
    {
        /** Node */
        GNode *node;
    };

    NavEventType type;
    NavEventData data;
public:
    /** Constructor from type only */
    inline NavEvent( NavEventType t): type( t)
    {
        data.node = NULL;
    }
    /** Constructor from type and node */
    inline NavEvent( NavEventType t, GNode *n): type( t)
    {
        data.node = n;
    }
    /** Get associated node */
    inline GNode *node() const
    {
        return data.node;
    }
    /** Set node */
    inline void setNode( GNode *n)
    {
        data.node = n;
    }
    /** Check if event is a focus on node */
    bool isFocus() const
    {
        return type == NAV_EVENT_NODE_FOCUS; 
    }
};

/**
 * History of graph view navigation
 * @ingroup GUIGraph
 */
class GraphViewHistory
{
    QList< NavEvent *> events; // List of events
    QList< NavEvent *>::Iterator it; // Iterator to hold current position in list
public:
    /** Constructor */
    GraphViewHistory(): events()
    {
        it = events.end();
    };
    /** Destructor */
    ~GraphViewHistory()
    {
        foreach ( NavEvent *ev, events)
        {
            delete ev;
        }
    }
    /** Get last event */
    NavEvent *last();
    /** Get prev event */
    NavEvent *prev();
    /** Get next event */
    NavEvent *next();
    /** New event */
    void viewEvent( NavEventType t, GNode *n = NULL);

    /** New focus event */
    inline void focusEvent( GNode *n)
    {
        assert( isNotNullP( n));
        viewEvent( NAV_EVENT_NODE_FOCUS, n);
    }
    /** Erase node from history */
    void eraseNode( GNode *n);
};

/**
 * Graph for graphics. Graph model layer of GraphView.
 * @ingroup GUIGraph
 */
class GGraph: public AuxGraph
{
    GraphView *view_p;
protected:
    Marker nodeTextIsShown;
    QList< GNode* > sel_nodes;
    QList< GEdge* > sel_edges;
    QHash< QString, GStyle *> styles;
    NodeNav node_in_focus;// Node in focus + navigation sector
    
    /** Node creation reimplementaiton */
    virtual Node * createNode( int _id)
    {
        GNode* node_p = new ( node_pool) GNode( this, _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual Edge * createEdge( int _id, Node *_pred, Node* _succ)
    {
        return new ( edge_pool) GEdge(  this, _id,
                                        static_cast<GNode *>( _pred), 
                                        static_cast<GNode *>( _succ));
    }
public:
    /** Constructor */
    inline GGraph( GraphView *v, bool create_pools):
        AuxGraph( false),
        view_p( v),
        node_in_focus( NULL, UNDEF_SECTOR)
    {
        nodeTextIsShown = newMarker();
 
        /** Pools' creation routine */
        if ( create_pools)
        {
            node_pool = new FixedPool< GNode>();
            edge_pool = new FixedPool< GEdge>();
        }
    }
    
    /** Destructor */
    virtual ~GGraph();

    /** New graphical node */
    virtual GNode* newNode();
    /** New graphical node */
    virtual GNode* newNode( QDomElement e);
    /** New graphical edge */
    virtual GEdge* newEdge( GNode* pred, GNode* succ);
    /** New graphical edge */
    virtual GEdge* newEdge( GNode* pred, GNode* succ, QDomElement e);
    
    /** Reimplementation of newEdge virtual function of base class */
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ));
    }
    /** Reimplementation of newEdge virtual function of base class */
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ, QDomElement e)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ), e);
    }
    /** Get graph's first edge */
    inline GEdge* firstEdge() 
    {
        return static_cast< GEdge *>( AuxGraph::firstEdge());
    }
    /** Get graph's first node */
    inline GNode* firstNode()
    {
        return static_cast< GNode *>( AuxGraph::firstNode());
    }
    /**
     * Run layout procedure
     */
    void doLayout();
    /**
     * Run layout procedure in single thread mode
     */
    void doLayoutSingle();

    /**
     * Assign placement coordinates to node items
     */
    void UpdatePlacement();
    /**
     * Reimplementation of layout's post processing step
     */
    void layoutPostProcess();

    /** Get corresponding graph view widget */
    inline GraphView *view() const
    {
        return view_p;
    }
    void showNodesText();
    /**
     * Add node to selection
     */
    inline void selectNode( GNode *n)
    {
        sel_nodes << n;
    }
    /** Get node in focus */
    inline GNode* nodeInFocus() const
    {
        return node_in_focus.node();
    }
    /** Get current navigation sector */
    inline NavSector nodeNavigationSector() const
    {
        return node_in_focus.sector();
    }
    /** Set node in focus */
    inline void setNodeInFocus( GNode *n, NavSector sector = UNDEF_SECTOR)
    {
        node_in_focus = NodeNav( n, sector);
    }
    void selectOneNode(GNode *n);
    /**
     * Clear list of selected nodes
     */
    inline void emptySelection()
    {
        foreach (GNode *n, sel_nodes)
        {
            n->item()->toRegular();
            n->item()->update();
        }
        sel_nodes.clear();
        sel_edges.clear();
    }
    /**
     * Add node to selection
     */
    inline void selectEdge( GEdge *e)
    {
        sel_edges << e;
    }
    /**
     * Delete scheduled nodes
     */
    void deleteNodes();

    /** Delete edge with all of the edge controls on it */
    void deleteEdgeWithControls( GEdge *e);

    /**
     * Delete scheduled edges
     */
    void deleteEdges();

    /**
     * Create self edge on selected node
     */
    void createSelfEdge();

    /**
     * Create label on selected edge
     */
    void createEdgeLabel( QPointF pos);

    /**
     * Create label on selected edge
     */
    void findContext();
    
    /**
     * Make all nodes of graph visible and eligible for placement
     */
    void showWholeGraph();

    /**
     * Zero nodes' priorities
     */
    void clearNodesPriority();

    /**
     * Show predecessor of selected edge
     */
    void showEdgePred();
    /**
     * Show successor of selected edge
     */
    void showEdgeSucc();

    /**
     * Save graph as an XML file
     */
    virtual void writeToXML( QString filename);

    /**
     * Build graph from XML description
     */
    virtual void readFromXML( QString txt);
    /**
     * Show style editor for selected nodes
     */
    void showEditNodeStyle();
    /**
     * Change node's style
     */
    void setNodeStyle( GStyle *style);
    /**
     * Show style editor for selected edges
     */
    void showEditEdgeStyle();
    /**
     * Change edge's style
     */
    void setEdgeStyle( GStyle *style);
    /**
     * Add a style
     */
    void addStyle( QString &name, GStyle *style)
    {
        styles[ name] = style;
    }
};

/**
 * View mode types
 */
enum GraphViewMode
{
    /** Whole graph view mode */
    WHOLE_GRAPH_VIEW,
    /** Context view mode */
    CONTEXT_VIEW,
    /** Number of view modes for graph */
    GRAPH_VIEW_MODES_NUM
};

/**
 * Graph visualization class
 */
class GraphView: public QGraphicsView
{
    Q_OBJECT; /** For MOC */
private:
    /** Pointer to model graph */   
    GGraph * graph_p;
    /** History of view events */
    GraphViewHistory *view_history;
    /** View mode */
    GraphViewMode view_mode;
    /** Item for showing future edge during the edge creation process */
    EdgeHelper *helper;
    /** Edition enabled/disabled */
    bool editable;
    /** Deleted items lists */
    QList< NodeItem* > del_node_items;
    QList< EdgeItem* > del_edge_items;

    /** Actions */
    QAction *editableSwitchAct;
    QAction *insertNodeAct;
    QAction *runLayoutAct;
    QAction *deleteItemAct;
    QAction *createSelfEdgeAct;
    QAction *createEdgeLabelAct;
    QAction *showPredAct;
    QAction *showSuccAct;
    QAction *findContextAct;
    QAction *showEditNodeStyleAct;
    QAction *showEditEdgeStyleAct;
    QAction *showTextAct;
    
    /** Context menus */
    QMenu *nodeItemMenu;
    QMenu *edgeItemMenu;
    
    /** Temporary data */
    int timer_id;
    int node_animation_timer;
    QPointF curr_pos;
    bool createEdge;
    bool show_menus;
    
    bool smooth_focus;
    bool smooth_layout_adjustment;

    bool zoom_out_done;
    GNode *tmpSrc;
    GNode *search_node;
    qreal zoom_scale;
    qreal preferred_zoom;


protected:
    void createActions();
    void createMenus();
signals:
    /** Signal that node is clicked */
    void nodeClicked( GNode *n);
public:
    /**
     * @brief Stores temporary info used for saving old style and node/edge being edited.
     * When showing style edit dialog we need to remember old state and the object
     * that is being edited. The StyleEditInfo structure is used for these purposes.
     */
    struct StyleEditInfo
    {
        GEdge* edge;
        GNode* node;
        GStyle *old_style;
        GStyle *new_style;
        StyleEdit* dialog;
        StyleEditInfo( GEdge* e, GStyle *olds, GStyle *news, StyleEdit* d):
            edge( e), node(NULL), old_style(olds), new_style( news), dialog( d)
        {}
        StyleEditInfo( GNode* n, GStyle *olds, GStyle *news, StyleEdit* d):
            edge( NULL), node(n), old_style(olds), new_style( news), dialog( d)
        {}
    };
protected:
    /** Temporary info for style edition */
    StyleEditInfo *style_edit_info;
public slots:
    /** Navigate backward */
    void navPrev();
    /** Navigate forward */
    void navNext();
    /** Clear search node */
    void clearSearch();
    /** Delete one item     */
    void deleteSelected();
    /** create self edge on selected node */
    void createSESelected();
    /** Create edge label */
    void createEdgeLabel();
    /** Show edge predecessor */
    void showEdgePred();
    /** Show edge successor */
    void showEdgeSucc();
    /** Find node's context */
    void findContext();
    /** Toggle smooth focusing mode ( moving viewport to show user how to get from one node to anoter) */
    void toggleSmoothFocus( bool smooth);
    /** Toggle view mode */
    void toggleViewMode( bool context);
    /** Show text of the clicked node */
    void showSelectedNodesText();
    /** Enable/disable edition */
    void toggleEdition( bool e);
    /** Insert node in center of view */
    void insertNodeOnCenter();
    /** Show style editor for node */
    void showEditNodeStyle();
    /** Show style editor for edge */
    void showEditEdgeStyle();
    /** Run layout */
    void runLayout();
    /** Change node's style */
    void setNodeStyle( GStyle *style);
    /** Change edge's style */
    void setEdgeStyle( GStyle *style);
    /** Style edit finished */
    void styleEditFinished( int result);
public:
    QProgressDialog *dialog;
    /** Constants */
#ifdef _DEBUG
    static const int MAX_DELETED_ITEMS_COUNT = 100;
#else
    static const int MAX_DELETED_ITEMS_COUNT = 10000;
#endif
    /** Constructor */
    GraphView();
    /** Destructor */
    virtual ~GraphView();
    /** Get action for toggling edit mode */
    inline QAction* toggleEditableAction() const
    {
        return editableSwitchAct;
    }
    /** Return true if graph enabled for edition */
    inline bool isEditable() const
    {
        return editable;
    }
    /** Return true if graph enabled for edition */
    inline void setEditable( bool val = true)
    {
        editable = val;
    }
    /** Return true if smooth focus is enabled */
    inline bool hasSmoothFocus() const
    {
        return smooth_focus;
    }
    
    /** Return true if view operates in context mode */
    inline bool isContext() const
    {
        return view_mode == CONTEXT_VIEW;
    }
    /** Return saved position */
    inline QPointF currPos() const
    {
        return curr_pos;
    }
    /** Set current event position */
    inline void setCurrPos( QPointF p)
    {
        curr_pos = p;
    }
    /** Get view history */
    inline GraphViewHistory * viewHistory() const
    {
        return view_history;
    }
    /** Get current search point */
    inline GNode * searchNode() const
    {
        return search_node;
    }
    /** Show text of the clicked node */
    inline void showNodeText( GNode * n)
    {
        emit nodeClicked( n);
    }

    /** Set model graph */
    inline void setGraph( GGraph *g)
    {
        graph_p = g;
    }
    /** Get pointer to model graph */
    inline GGraph *graph() const
    {
        return graph_p;
    }
    /** Get context menu for nodes */
    inline QMenu *nodeMenu() const
    {
        return nodeItemMenu;
    }
    /** Get context menu for edges */
    inline QMenu *edgeMenu() const
    {
        return edgeItemMenu;
    }

    /** Create menu for particular node */
    virtual QMenu* createMenuForNode( GNode *n);
    
    /** Create menu for particular edge */
    virtual QMenu* createMenuForEdge( GEdge *e);

    void dragEnterEvent( QDragEnterEvent *event);

    void dropEvent( QDropEvent *event);

    void dragMoveEvent( QDragMoveEvent *event);

    /** draw background reimplementation */
    void drawBackground( QPainter *painter, const QRectF &rect);
    /** Mouse double click event handler reimplementation */
    void mouseDoubleClickEvent( QMouseEvent *event);
    /** Mouse press event handler reimplementation */
    void mousePressEvent( QMouseEvent *event);
    /** Mouse move event handler reimplementation */
    void mouseMoveEvent( QMouseEvent *event);
    /** Mouse release event handler reimplementation */
    void mouseReleaseEvent( QMouseEvent *event);
    /** Context menu event handler */
    void contextMenuEvent( QContextMenuEvent * e );
    /** Keypress event handler reimplementation */
    void keyPressEvent( QKeyEvent *event);

    /** Mouse wheel event handler reimplementation */
    void wheelEvent( QWheelEvent *event);
    /** Zoom the view in */
    void zoomIn();
    /** Zoom the view out */
    void zoomOut();
    /** Restore original zoom */
    void zoomOrig();
    /** Do the transofrmation( scale) */
    void updateMatrix();
    /** Focus on node */
    void focusOnNode( GNode *n, bool gen_event);
    /** Start animation for nodes */
    void startAnimationNodes();
    /** Advance view in animated focusing procedure */
    void advanceView();
    /** Advance nodes in animation sequence */
    void advanceNodes();
    /** Replay navigation event */
    void replayNavigationEvent( NavEvent *event);
    /** Check if we are in the process of the edge creation */
    inline bool isCreateEdge() const
    {
        return createEdge;
    }
    /** Check if we are in the process of the edge creation */
    inline bool isShowContextMenus() const
    {
        return show_menus;
    }
    /** Set the state of ege creation */
    inline void SetCreateEdge( bool val)
    {
        createEdge = val;
    }
    /** Save the pointer to source node for new edge */
    inline void SetTmpSrc( GNode* node)
    {
        tmpSrc = node;
        helper->setSrc( node->item());
        helper->setDst( NULL);
        helper->adjust();
    }
    /** Show edge helper item */
    inline void showHelper()
    {
        helper->adjust();
        helper->show();
    }
    /** Get the pointer to source node of new edge */
    inline GNode* GetTmpSrc()
    {
        return tmpSrc;
    }

    /**
     * Schedule node item for deletion
     */
    void deleteLaterNodeItem( NodeItem *item)
    {
        del_node_items << item;
        checkDelItems();
    }
    /**
     * Schedule edge item for deletion
     */
    void deleteLaterEdgeItem( EdgeItem *item)
    {
        del_edge_items << item;
        checkDelItems();
    }
    
    /** Timer event handler */
    void timerEvent(QTimerEvent *event);
     
    /**
     * Delete items that have been disconnected from scene
     */
    void deleteItems();
    
    /**
     * Find node by its ID from dump
     */
    GNode* findNodeById( int id);

     /**
     * Find node by its label
     */
    GNode* findNodeByLabel( QString label);

    /**
     * Find next node with matching text
     */
    GNode *findNextNodeWithText( QString &findStr, QTextDocument::FindFlags flags);

    /**
     * Find prev node with matching text
     */
    GNode *findPrevNodeWithText( QString &findStr, QTextDocument::FindFlags flags);

    /** 
     * Check that we haven't exceeded the max amount of deleted items
     */
    inline void checkDelItems()
    {
        int item_count = del_node_items.count() + del_edge_items.count();
        if ( item_count >= MAX_DELETED_ITEMS_COUNT)
        {
            /** deleteItems(); !!! FIXME: MEMORY LEACKAGE( yes, not potential... known leakage) */
        }
    }
    /** Set the pointer to style edition temporary info */
    inline void setStyleEditInfo( StyleEditInfo* info)
    {
        style_edit_info = info;
    }

};

#endif
