/**
 * @file: node_item.h 
 * Drawable Node class definition.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

/**
 * Subclass of QGraphicsItem for representing a node in scene
 * @ingroup GUIGraph
 */
class NodeItem: public QGraphicsTextItem
{    
    GNode *node_p;
	QDockWidget *text_dock;
    qreal opacity;
    bool bold_border;
    bool alternate_background; 

    /** Initialization */
    void SetInitFlags();
public:
    /** Type of graphics item that corresponds to node */
    enum {Type = TypeNode};

    /** Constructor */
	inline NodeItem( GNode *n_p):
        opacity( MAX_OPACITY),
        bold_border( false),
        alternate_background( false)
    {
        node_p = n_p;
        SetInitFlags();
    }
    /** Get item's opacity level */
    inline qreal opacityLevel() const
    {
        return opacity;
    }
    /** Set item's opacity level */
    inline void setOpacityLevel( qreal op_l)
    {
        opacity = op_l;
    }
    /** Set node to be highlighted */
    inline void highlight()
    {
        bold_border = true;
        alternate_background = true;
        update();
    }
    /** Set node to be regular */
    inline void toRegular()
    {
        bold_border = false;
        alternate_background = false;
        update();
    }
    /** Get corresponding text doc */
	inline QDockWidget *textDock() const
	{
		return text_dock;
	}
	/** Set corresponding text doc */
	inline void setTextDock( QDockWidget *dock)
	{
		text_dock = dock;
	}
	/** Get pointer to model node */
    inline GNode *node() const
    {
        return node_p;
    }
    /** Get the node type */
    inline int type() const
    {
        return Type;
    }
    /** Get text border rectangle */
    inline QRectF textRect() const
    {
        return QGraphicsTextItem::boundingRect();
    }
    /** Get the inner border rectangle */
    QRectF borderRect() const;
    /** Get the bounding rectangle */
    QRectF boundingRect() const;
    /** Get node shape */
    QPainterPath shape() const;
    /** Determine that node's shape contains point */
    bool contains(const QPointF &point) const;
    /** Paint procedure */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /** Reimplementation of mouse press event handler */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of mouse release event handler */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of double click event handler */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of focus out event */
    void focusOutEvent(QFocusEvent *event);
    /** Reimplementation of key press event */
    void keyPressEvent(QKeyEvent *event);
    /** Adjust associated items */
    void adjustAssociates();
    /** Update associated items */
    void updateAssociates();
    /** Item change event handler */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	/** 
     * Perform animation step
     * Advance node's coordinates and opacity towards goal values of these parameters
     * Return true if node have advanced somehow. False if node hasn't change
     */
    bool advance();
    
    /** Remove from scene */
    inline void remove()
    {
        setVisible( false);
        removeFromIndex();
        scene()->removeItem( this);
        node_p = NULL;
    }

    /** Change shape */
    void shapeChanged();
};
/**
 * Representation of model graph node
 * @ingroup GUIGraph
 */
class GNode: public AuxNode
{
    GraphNum ir_id;
	bool text_shown;
	/** Representation of node in graph view */
    NodeItem *item_p; 
    /** Graphical appearance style */
    GStyle *_style;

    /** Representation of node as text */
    QTextDocument* _doc;
protected:    
    /** We can't create nodes separately, do it through newNode method of graph */
    GNode( GGraph *graph_p, int _id);
    /** Contructor of node with specified position */
    GNode( GGraph *graph_p, int _id, QPointF _pos);

    friend class GGraph;
    
public:
    /** Destructor */
    virtual ~GNode();
	
	/** Tell whether the text is shown */
	inline bool isTextShown() const
	{
		return text_shown;
	}
    /** Check if this node is in focus */
    bool isNodeInFocus() const;
    
    /** Memorize that the text is shown */
	inline void setTextShown( bool shown = true)
	{
		text_shown = shown;
	}

	/** Get node's Id as it was parsed from ir dump */
	inline GraphNum irId() const
	{
		return ir_id;
	}
	/** Set node's IR ID */
	inline void setIRId( GraphNum i)
	{
		ir_id = i;
	}

	/** Get the pointer to item */
    inline NodeItem* item() const
    {
        return item_p;
    }
    /** Get the corresponding text */
    inline QTextDocument *doc() const
    {
        return _doc;
    }
    /** Set node's text document */
    inline void setDoc( QTextDocument* doc)
    {
        _doc = doc;
    }
    
    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    GGraph * graph() const;
    /** Get next graph's node */
    inline GNode* nextNode()
    {
        return static_cast< GNode*>( AuxNode::nextNode());
    }
    /** Get prev graph's node */
    inline GNode* prevNode()
    {
        return static_cast< GNode*>( AuxNode::prevNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( GEdge *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    /** Add predecessor */
    inline void AddPred( GEdge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    /** Add successor */
    inline void AddSucc( GEdge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /** Get first edge in given direction */
    inline GEdge* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< GEdge*>( AuxNode::firstEdgeInDir( dir));
    }
    /** Get first successor */
    inline GEdge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Get first predecessor */
    inline GEdge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    /** Get node's width */
    virtual inline double width() const
    {
        return item()->borderRect().width();
    }

    /** Get node's nodes height */
    virtual inline double height() const
    {
        return item()->boundingRect().height();
    }
    /** Set node's style */
    inline void setStyle( GStyle *st)
    {
        if ( isNotNullP( _style))
            _style->decNumItems();
        _style = st;
        if ( isNotNullP( _style))
            _style->incNumItems();
        item()->update();
    }
    /** Get node's style */
    inline GStyle * style() const
    {
        return _style;
    }
};
#endif