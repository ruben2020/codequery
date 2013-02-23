/**
 * @file: navigation.h 
 * Interfaces for graph navigation
 * @ingroup GUIGraph
 */
/**
 * @page navigation Navigation Support Classes
 * 
 * NodeNav
 * NodeNav class is used for browsing graph with keyboard. Conceptually
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "gview_impl.h"

/**
 * Directions around node: four sectors between
 * diagonals of node's bounding rectangle
 */
enum NavSector
{
    /** Sector on the top of the node */
    TOP_SECTOR,
    /** Sector in the bottom of the node */
    BOTTOM_SECTOR,
    /** Sector to the left of the node */
    LEFT_SECTOR,
    /** Sector to the right of the node */
    RIGHT_SECTOR,
    /** Undefined sector */
    UNDEF_SECTOR
};

/**
 * Directions around node: four sectors between
 * diagonals of node's bounding rectangle
 */
enum NavDirection
{
    /** Upward direction of navigation */
    NAV_DIR_UP,
    /** Downward direction of navigation */
    NAV_DIR_DOWN,
    /** Left direction of navigation */
    NAV_DIR_LEFT,
    /** Right direction of navigation */
    NAV_DIR_RIGHT
};

/**
 * Navigation around node: edge selection and jump to one of its endpoints
 * @ingroup GUIGraph
 */
class NodeNav
{
public:
    /** Constructor */
    NodeNav( GNode *curr_node, NavSector nav_sector);

    /** Get edge to the left of given edge */
    GEdge *firstEdgeInSector() const;
    /** Get edge in given direction of given edge */
    GEdge *edgeInDir( GEdge * edge, NavDirection dir) const;
    /** Check that given edge is in current sector */
    bool isEdgeInSector( GEdge * edge) const;    
    
    /** Get node */
    inline GNode* node() const;
    /** Get sector */
    inline NavSector sector() const;
    /** Check if navigation direction is applicable in current sector */
    static bool isDirApplicable( NavDirection dir, NavSector s);
    /** Check that given point is in given direction from reference point */
    static bool isPointInDir( QPointF point, QPointF ref, NavDirection dir);
    /** Compute coordinate difference between points in given direction */
    static qreal deltaInDir( QPointF point, QPointF ref, NavDirection dir);

private:    

    /** Compute max angle for sector */
    qreal sectorMaxAngle() const;
    /** Compute min angle for sector */
    qreal sectorMinAngle() const;
    /**
     * Get node on other end of edge
     *  NOTE:if node_priv is not adjacent to edge then returns NULL
     */
    GNode* otherEnd( GEdge *) const;

    GNode *node_priv;
    NavSector sector_priv;
};

/** Get pred */
GNode* NodeNav::node() const
{
    return node_priv;
}
/** Get succ */
NavSector NodeNav::sector() const
{
    return sector_priv;
}

#endif /** NAVIGATION_H */