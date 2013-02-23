/**
 * @file: aux_edge.h
 * Layout Edge class
 *
 * Graph for performing layouts, consists of dummy nodes.
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * AuxGraph, AuxNode and AuxEdge classes represent auxiliary graph used for layout purposes
 */
#ifndef AUX_EDGE_H
#define AUX_EDGE_H
/**
 * Edge types
 *
 * @ingroup Layout
 */
enum AuxEdgeType
{
	/** Unknown type (typically before classification) */
	UNKNOWN_TYPE_EDGE = 0,

	/** Tree edge */
	TREE_EDGE,
	
	/** Back edge */
	BACK_EDGE,

	/** Forward edge */
	FORWARD_EDGE,

	/** Self-edge */
	SELF_EDGE,

	/** Number of edge types */
	EDGE_TYPES_NUM
};

/**
 * Edge of model graph used in Layout
 *
 * @ingroup Layout
 */
class AuxEdge: public Edge
{
    bool priv_fixed;

    AuxEdgeType priv_type;
   
protected:
    /** Constructors are made private, only nodes and graph can create edges */
    AuxEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ);
public:
    friend class AuxGraph;
    friend class AuxNode;

    /** Get node in given direction */
    inline AuxNode *node( GraphDir dir) const
    {
        return static_cast< AuxNode *>( Edge::node( dir));
    }
    /** Get predecessor */
    inline AuxNode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline AuxNode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }  
    /** insert node on this edge */
    inline AuxNode *insertNode()
    {
        return static_cast< AuxNode *>( Edge::insertNode());
    }
    /** Next edge in graph's list */
    inline AuxEdge* nextEdge()
    {
        return static_cast< AuxEdge *>( Edge::nextEdge());
    }
    /** Next edge in give direction */
    inline AuxEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< AuxEdge *>( Edge::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline AuxEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline AuxEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
    /** Check if an edge is fixed */
    inline bool isFixed() const
    {
        return priv_fixed;
    }
    /** Set edge to be fixed */
    inline void setFixed( bool fx)
    {
        priv_fixed = fx;
    }
    /** Check if edge was classified as a 'Backedge' */
    inline bool isBack() const
    {
        return priv_type == BACK_EDGE;
    }
	/** Get edge type */
	inline AuxEdgeType type() const
	{
		return priv_type;
	}
    /** Set edge type */
    inline void setType( AuxEdgeType t = UNKNOWN_TYPE_EDGE)
    {
		if ( isSelf())
		{
			return;
		}
		priv_type = t;
    }
	/** Set edge to be of unknown type */
    inline void setUnknown()
    {
        setType( UNKNOWN_TYPE_EDGE);
    }
	/** Set edge to be a tree edge */
    inline void setTree()
    {
       setType( TREE_EDGE);
    }
	/** Set edge to be a backedge */
    inline void setBack()
    {
        setType( BACK_EDGE);
    }
    /** Set edge to be a forward edge */
    inline void setForward()
    {
        priv_type = FORWARD_EDGE;
    }
	/** Set edge to be a self-edge */
    inline void setSelf()
    {
        assertd( pred() == succ());
		setType( SELF_EDGE);
    }
    /** Check if edge is a self-edge */
    inline bool isSelf() const
    {
        return pred() == succ();
    }
	/** Check whether this edge is inverted */
    inline bool isInverted() const
    {
        return isBack();
    }
	/** Return real node in given direction */
	inline AuxNode* realNode( GraphDir dir) const
	{
		AuxNode* n = node( dir);
		assertd( isNotNullP( n));
		while ( n->isPseudo())
		{
			n = n->firstEdgeInDir( dir)->node( dir);
		}
		return n;
	}
	/** Get real predecessor */
	inline AuxNode* realPred() const
	{
		return realNode( GRAPH_DIR_UP);
	}
	/** Get real successor */
	inline AuxNode* realSucc() const
	{
		return realNode( GRAPH_DIR_DOWN);
	}
};

#endif /* AUX_EDGE_H */