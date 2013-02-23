/**
 * @file: aux_node.h
 * Layout Node class
 *
 * Graph for performing layouts, consists of dummy nodes.
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * AuxGraph, AuxNode and AuxEdge classes represent auxiliary graph used for layout purposes
 */
#ifndef AUX_NODE_H
#define AUX_NODE_H

/**
 * Types of a node
 *
 * @ingroup Layout
 */
enum AuxNodeType
{
    /* Simple aux node that represents one node of processed graph */
    AUX_NODE_SIMPLE,
    /* Aux node that represents a control point of edge in processed graph */
    AUX_EDGE_CONTROL,
    /** Aux node that represents an edge label */
    AUX_EDGE_LABEL,
    /* Number of aux node types */
    AUX_NODE_TYPES_NUM
};

/**
 * Represents nodes and edge controls in Layout
 * 
 * @ingroup Layout
 */
class AuxNode: public Node
{
    double priv_x;
    double priv_y;
    double priv_height;
    double priv_width;
    qreal barycenter;
    int priv_priority;
    Level * priv_level;
    int priv_order;
    Rank priv_rank;
    AuxNodeType node_type;
    bool is_for_placement;
    bool stable;
public:
        /** Get next graph's node */
    inline AuxNode* nextNode()
    {
        return static_cast< AuxNode*>( Node::nextNode());
    }
    /** Get prev graph's node */
    inline AuxNode* prevNode()
    {
        return static_cast< AuxNode*>( Node::prevNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( AuxEdge *edge, GraphDir dir);
    /** Add predecessor */
    inline void AddPred( AuxEdge *edge);
    /** Add successor */
    inline void AddSucc( AuxEdge *edge);
    /** Get first edge in given direction */
    inline AuxEdge* firstEdgeInDir( GraphDir dir);
    /** Get first successor */
    inline AuxEdge* firstSucc();
    /** Get first predecessor */
    inline AuxEdge* firstPred();
    /** Check is node should be 'more stable' during layout process */
    inline bool isStable() const
    {
        return stable;
    }
    /** Set node to be 'more stable' during layout process */
    inline void setStable( bool st = true)
    {
        stable = st;
    }
    /** Get Height */
    virtual double height() const
    {
        return priv_height;
    }
    /** Get Width */
    virtual double width() const
    {
        return priv_width;
    }
    /** Get x coordinate */
    inline double modelX() const
    {
        return priv_x;
    }
    /** Get y coordinate */
    inline double modelY() const
    {
        return priv_y;
    }
    /** Get node's priority */
    inline int priority() const
    {
        return priv_priority;
    }
    /** Get rank */
    inline int rank() const
    {
        return priv_rank;
    }
    /** Get order */
    inline int order() const
    {
        return priv_order;
    }
    /** Set horizontal coordinate */
    inline void setX( double x)
    {
        priv_x = x;
    }
    /** Set vertical coordinate */
    inline void setY( double y)
    {
        priv_y = y;
    }
    /** Get barycenter horizontal coordinate */
    inline qreal bc() const
    {
        return barycenter;
    }
    /** Set barycenter horizontal coordinate */
    inline void setBc( qreal center)
    {
        barycenter = center;
    }
    /** Set Height */
    inline void setHeight( double h) 
    {
        priv_height = h;
    }
    /** Set Width */
    inline void setWidth( double w) 
    {
        priv_width = w;
    }
    /** Set priority */
    inline void setPriority( int p) 
    {
        priv_priority = p;
    }
    /** Set level */
    inline void setLevel( Level* l) 
    {
        priv_level = l;
    }
    /** Get level */
    inline Level *level() const
    {
        return priv_level;
    }
    /** Set order */
    inline void setOrder( int order)
    {
        priv_order = order;
    }
    /** Set rank */
    inline void setRank( Rank r)
    {
        priv_rank = r;
    }
    /** Set type */
    inline void setType( AuxNodeType t)
    {
        node_type = t;
    }
    /** Get type */
    inline AuxNodeType type() const
    {
        return node_type;
    }
    /** Check if this node is a simple one */
    inline bool isSimple() const
    {
        return node_type == AUX_NODE_SIMPLE;
    }
    /** Check if this node is an edge control */
    inline bool isEdgeControl() const
    {
        return node_type == AUX_EDGE_CONTROL;
    }
    /** Check if this node is an edge control */
    inline bool isEdgeLabel() const
    {
        return node_type == AUX_EDGE_LABEL;
    }
    /** Check whether node is pseudo */
    inline bool isPseudo() const
    {
        return isEdgeControl() || isEdgeLabel();
    }
    /** Set node to be an edge control */
    inline void setTypeEdgeControl()
    {
        node_type = AUX_EDGE_CONTROL;
    }
    /** Set node's type to simple */
    inline void setTypeSimple()
    {
        node_type = AUX_NODE_SIMPLE;
    }
    /** Set node's type to simple */
    inline void setTypeEdgeLabel()
    {
        node_type = AUX_EDGE_LABEL;
    }
    /** Check if this node participates in horizontal arrangement */
    inline bool isForPlacement() const
    {
        return is_for_placement;
    }
    /** Set node to participate in horizontal arrangement */
    inline void setForPlacement( bool placed = true)
    {
        is_for_placement = placed;
    }
    /** Print node's debug info */
    inline void debugPrint()
    {
        switch( node_type)
        {
            case AUX_NODE_SIMPLE:
                out("SIMPLE %llu;", id());
                break;
            case AUX_EDGE_CONTROL:
                out("EDGE CONTROL %llu;", id());
                break;
            case AUX_EDGE_LABEL:
                out("EDGE LABEL %llu;", id());
                break;
            default:         
                assert( 0);
                out("NO_TYPE %llu;", id());
                break;
        }
    }

    /**
     * Return value of spacing between previous and current node due to their types
     */
    inline qreal spacing( AuxNodeType prev_type) const
    {
        switch ( prev_type)
        {
            case AUX_NODE_SIMPLE:
                if ( node_type == AUX_NODE_SIMPLE)
                {
                    return NODE_NODE_MARGIN;
                } else
                {
                    return NODE_CONTROL_MARGIN;
                }
            case AUX_EDGE_CONTROL:
                if ( node_type == AUX_NODE_SIMPLE)
                {
                    return NODE_CONTROL_MARGIN;
                } else
                {
                    return CONTROL_CONTROL_MARGIN;
                }
            case AUX_EDGE_LABEL:
                if ( node_type == AUX_NODE_SIMPLE)
                {
                    return NODE_NODE_MARGIN;
                } else
                {
                    return NODE_CONTROL_MARGIN;
                }
            case AUX_NODE_TYPES_NUM:
                return 0;
        }
        return NODE_NODE_MARGIN;
    }
    
    /** Destructor */
    virtual ~AuxNode();

protected:
    /** We can't create nodes separately, do it through newNode method of graph */
    AuxNode( AuxGraph *graph_p, int _id);
    
    friend class AuxGraph;
};



#endif /* AUX_NODE_H */