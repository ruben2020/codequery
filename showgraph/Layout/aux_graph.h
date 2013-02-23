/**
 * @file: aux_graph.h
 * Layout Graph class
 *
 * Graph for performing layouts, consists of dummy nodes.
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * AuxGraph, AuxNode and AuxEdge classes represent auxiliary graph used for layout purposes
 */
#ifndef AUX_GRAPH_H
#define AUX_GRAPH_H

#include "layout_iface.h"

/** Get first edge in given direction */
inline AuxEdge*
AuxNode::firstEdgeInDir( GraphDir dir)
{
    return static_cast< AuxEdge*>( Node::firstEdgeInDir( dir));
}
/** Get first successor */
inline AuxEdge*
AuxNode::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** Get first predecessor */
inline AuxEdge*
AuxNode::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/** Edge connection reimplementation */
inline void
AuxNode::AddEdgeInDir( AuxEdge *edge, GraphDir dir)
{
    Node::AddEdgeInDir( edge, dir);
}
/** Add predecessor */
inline void
AuxNode::AddPred( AuxEdge *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}
/** Add successor */
inline void
AuxNode::AddSucc( AuxEdge *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
/**
 * Graph with nodes of two types: simple nodes and edge controls
 *
 * @ingroup Layout
 */
class AuxGraph: public QObject, public Graph
{
    Q_OBJECT;
private:
    /** Layout algorithm inner variables */
    bool layout_in_process;
    int cur_level; //last processed level
    int cur_pass; // current pass
    
    /** Watcher for processing layout in parallel with main event loop */
    QFutureWatcher< void> *watcher;

    /** Array of node lists for ranks */
    QVector< Level*> levels;

    /** Order numeration */
    Numeration order;
    
    /** Ranking state */
    bool ranking_valid;

    /** Ranking numeration */
    Numeration ranking;

    /** Maximum used for ranking */
    GraphNum max_rank;

    /**
     * Structure used for dfs traversal loop-wise implementation
     * Not part of interface used internally
     */
    struct SimpleDfsStepInfo
    {
        AuxNode *node; // Node in consideration
        AuxEdge *edge; // Next edge

        /* Constructor */
        SimpleDfsStepInfo( AuxNode *n)
        {
            node = n;
            edge = n->firstSucc();
        }
        /* Constructor with direction specification */
        SimpleDfsStepInfo( AuxNode *n, GraphDir dir)
        {
            node = n;
            edge = n->firstEdgeInDir( dir);
        }
    };

    /**
     * Find enters for graph so that
     * for every node there would exist an enter from wich it is reachable
     */
    QStack< SimpleDfsStepInfo *> findEnterNodes();

    /**
     * Mark nodes that are reachable down from given node
     */
    GraphNum markReachableDown( AuxNode *n, Marker m);

    /** Initialize levels */
    void initLevels( Rank max_level);
    /** Delete levels */
    void deleteLevels();
    /** Set order of every node using DFS */
    void orderNodesByDFS();
    /** Try to reduce crossings */
    void reduceCrossings();
    /** Find proper vertical position for each level */
    void adjustVerticalLevels();

    /** Assign ranks to nodes in respect to maximum length of path from top */
    Numeration rankNodes();
    /** Assign edge types, mark edges that should be inverted */
    void classifyEdges();

    /** Get numeration that describes ranks in graph */
    inline Numeration ranks() const
    {
        return ranking;
    }
    /** Get max rank number */
    inline GraphNum maxRank() const
    {
        return max_rank;
    }
protected:
    /** Create node overload */
    virtual Node * createNode( int _id)
    {
        return new ( node_pool) AuxNode( this, _id);
    }
    /** Create edge overload */
    virtual Edge * createEdge( int _id, Node *_pred, Node* _succ)
    {
        return new ( edge_pool) AuxEdge( this, _id, 
                                         static_cast<AuxNode *>( _pred),
                                         static_cast<AuxNode *>( _succ) );
    }
    /** Get node that is considered root one after the layout */
    AuxNode* rootNode();
    /** Arrange nodes horizontally */
    void arrangeHorizontally();
    /** Arrange nodes horizontally without respect of stable nodes */
    void arrangeHorizontallyWOStable();
    /** Arrange nodes horizontally with respect of stable nodes */
    void arrangeHorizontallyWithStable( Rank min, Rank max);
public slots:
    /** Process next level while doing layout in parallel with main event loop */
    void layoutNextStep();
signals:
    /** signal some progess in layout process */
    void progressChange( int value);
    /** signal that layout has finished */
    void layoutDone();
public:
    /** Default constructor */
    AuxGraph( bool create_pools);
    /** Destructor */
    virtual ~AuxGraph();

    /** Get graph's first edge */
    inline AuxEdge* firstEdge() 
    {
        return static_cast< AuxEdge *>( Graph::firstEdge());
    }
    /** Get graph's first node */
    inline AuxNode* firstNode()
    {
        return static_cast< AuxNode *>( Graph::firstNode());
    }

    /** Check if ranking is is valid */
    inline bool rankingValid() const
    {
        return ranking_valid;
    }

    /** Set ranking to invalid state */
    inline void invalidateRanking()
    {
        ranking_valid = false;
    }
    /** Set ranking to valid state */
    inline void validateRanking()
    {
        ranking_valid = true;
    }

    /** Debug info print */
    virtual void debugPrint()
    {
        out( "AuxGraph debug print");
        Graph::debugPrint();
    }
    
    /** Perform layout */
    void doLayout();
    
    /** Perform layout using concurrent threads */
    void doLayoutConcurrent();
    
    /**
     * Run some actions after main layout algorithm
     */
    virtual void layoutPostProcess();
};

/**
 * Representation of rank level
 *
 * @ingroup HLayout
 * A level rpresents a group of nodes that should have same/close vertical position
 */
class Level
{
    qreal _height;
    qreal y_pos;
    Rank level_rank;
    QList< AuxNode*> node_list;
public:
    /** Default constructor */
    inline Level(): level_rank( 0), node_list(), _height( 0), y_pos( 0){};
    /** Constructor with rank parameter */
    inline Level( Rank r): level_rank( r), node_list(), _height( 0), y_pos( 0){};
    /** Arrange nodes with respect to adjacent level*/
    void arrangeNodes( GraphDir dir, bool commit_placement, bool first_pass);
    /** Sort nodes by their order */
    void sortNodesByOrder();
    /** Get level's rank */
    inline Rank rank() const
    {
        return level_rank;
    }
    /** Set level's rank */
    inline void setRank( Rank r)
    {
        level_rank = r;
    }
    /** Get node list */
    inline QList< AuxNode*> nodes() const
    {
        return node_list;
    }
    /** Add a node to list */
    inline void add( AuxNode *node)
    {
        node_list.push_back( node);
        if ( _height < node->height())
            _height = node->height();
        node->setLevel( this);
        node->setRank( level_rank);
    }
    /** Set level height */
    inline void setHeight( qreal h)
    {
        _height = h;
    }
    /** Get level height */
    inline qreal height() const
    {
        return _height;
    }
    /** Set level vertical position */
    inline void setY( qreal yy)
    {
        y_pos = yy;
    }
    /** Get level vertical pos */
    inline qreal y() const
    {
        return y_pos;
    }
};

/** Constructors are made private, only nodes and graph can create edges */
inline AuxEdge::AuxEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ):
    Edge( graph_p, _id, _pred, _succ),
    priv_fixed( true), priv_type( UNKNOWN_TYPE_EDGE) 
{
    if ( _pred == _succ)
        priv_type = SELF_EDGE;
};

/** Constructor for node */
inline AuxNode::AuxNode( AuxGraph *graph_p, int _id):
    Node( graph_p, _id),
    priv_x(0),
    priv_y(0),
    priv_height(0),
    priv_width(0),
    priv_priority(-1),
    priv_level( NULL),
    priv_order(-1),
    node_type( AUX_NODE_SIMPLE),
    is_for_placement( true),
    stable( false)
{
}
#endif /** AUX_GRAPH_H */
