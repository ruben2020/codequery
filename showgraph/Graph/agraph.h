/**
 * @file: agraph.h 
 * Abstract Graph for testing of graph's properties and usage model.
 *
 * @defgroup AGr Test Graph
 *
 * @ingroup GraphBase
 * AGraph, ANode and AEdge classes present mimnimal code
 * that you need to write to employ Graph Library functionality.
 * AGraph classes differ from base in only one member of ( int) type
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef AGRAPH_H
#define AGRAPH_H

/* Predeclarations */
class ANode;
class AEdge;
class AGraph;

/**
 * Abstract node
 *
 * @ingroup AGr
 */
class ANode: public Node
{
    int dummy;
    /** We can't create nodes separately, do it through newNode method of graph */
    ANode( AGraph *graph_p, int _id);
    friend class AGraph;
public:
        /** Get next graph's node */
    inline ANode* nextNode()
    {
        return static_cast< ANode*>( Node::nextNode());
    }
    /** Get prev graph's node */
    inline ANode* prevNode()
    {
        return static_cast< ANode*>( Node::prevNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( AEdge *edge, GraphDir dir);
    /** Add predecessor */
    inline void AddPred( AEdge *edge);
    /** Add successor */
    inline void AddSucc( AEdge *edge);
    /** Get first edge in given direction */
    inline AEdge* firstEdgeInDir( GraphDir dir);
    /** Get first successor */
    inline AEdge* firstSucc();
    /** Get first predecessor */
    inline AEdge* firstPred();

};

/**
 * Abstract edge
 *
 * @ingroup AGr
 */
class AEdge: public Edge
{
    int dummy;

    /** Constructors are made private, only nodes and graph can create edges */
    AEdge( AGraph *graph_p, int _id, ANode *_pred, ANode* _succ);
        
    friend class AGraph;
    friend class ANode;
public:
        /** Get node in given direction */
    inline ANode *node( GraphDir dir) const
    {
        return static_cast< ANode *>( Edge::node( dir));
    }
    /** Get predecessor */
    inline ANode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline ANode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }  
    /** insert node on this edge */
    virtual ANode *insertNode()
    {
        return static_cast< ANode *>( Edge::insertNode());
    }
    /** Next edge in graph's list */
    inline AEdge* nextEdge()
    {
        return static_cast< AEdge *>( Edge::nextEdge());
    }
    /** Next edge in give direction */
    inline AEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< AEdge *>( Edge::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline AEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline AEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

/**
 * Testing-purpose graph
 *
 * @ingroup AGr
 */
class AGraph: public Graph
{
    int dummy; //Dummy class member


    /** Node creation overload */
    Node * createNode( int _id);
	/** Edge creation overload */
    Edge * createEdge( int _id, Node *_pred, Node* _succ);

    public:
            
    /** New graphical node */
    ANode* newNode()
    {
        return static_cast< ANode*>( Graph::newNode());
    }

    /** New graphical edge */
    AEdge* newEdge( ANode* pred, ANode* succ)
    {
        return static_cast< AEdge *>( Graph::newEdge( pred, succ));
    }

    /** Get graph's first edge */
    inline AEdge* firstEdge() 
    {
        return static_cast< AEdge *>( Graph::firstEdge());
    }
    /** Get graph's first node */
    inline ANode* firstNode()
    {
        return static_cast< ANode *>( Graph::firstNode());
    }
    /** Pools' creation routine */
    void createPools()
    {
        node_pool = new FixedPool< ANode>();
        edge_pool = new FixedPool< AEdge>();
    }
    /** Constructor */
    AGraph( bool create_pools): Graph( false)
    {
        if ( create_pools)
            createPools();
    }
};

/** Node constructor */
inline ANode::ANode( AGraph *graph_p, int _id):
        Node( graph_p, _id)
{

}

/** Edge constructor */
inline AEdge::AEdge( AGraph *graph_p, int _id, ANode *_pred, ANode* _succ):
        Edge( graph_p, _id, _pred, _succ)
{

}
        
/** Node creation overload */
inline Node * AGraph::createNode( int _id)
{
    return new ( nodePool()) ANode( this, _id);
}
/** Edge creation overload */
inline Edge * AGraph::createEdge( int _id, Node *_pred, Node* _succ)
{
    return new ( edgePool()) AEdge( this, _id, static_cast<ANode*>( _pred), static_cast< ANode *>(_succ));
} 

/** Get first edge in given direction */
inline AEdge*
ANode::firstEdgeInDir( GraphDir dir)
{
    return static_cast< AEdge*>( Node::firstEdgeInDir( dir));
}
/** Get first successor */
inline AEdge*
ANode::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** Get first predecessor */
inline AEdge*
ANode::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/** Edge connection reimplementation */
inline void
ANode::AddEdgeInDir( AEdge *edge, GraphDir dir)
{
    Node::AddEdgeInDir( edge, dir);
}
/** Add predecessor */
inline void
ANode::AddPred( AEdge *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}
/** Add successor */
inline void
ANode::AddSucc( AEdge *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
#endif
