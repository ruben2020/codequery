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
