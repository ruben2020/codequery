/**
 * @file: visible_edge.h 
 * Definition of VEdge class
 * @ingroup GUIGraph
 */
/**
 * @page vedge Visible Edge Class
 * 
 * Visible Edge Class
 * VEdge stands for "visible edge". It is a class for manipulating
 * an edge like the user see it. The need for this stems from
 * the fact that edge seen by user is actually composed of 
 * several low-level edges between pseudo nodes.
 */
/*
 * GUI for ShowGraph tool.
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
#ifndef VISIBLE_EDGE_H
#define VISIBLE_EDGE_H

#include "gview_impl.h"

/**
 * Visible edge
 * @ingroup GUIGraph
 */
class VEdge
{
public:
    /** Constructor from two nodes */
    VEdge( GNode *pred_node, GNode *succ_node);
    /** Constructor from existing graph edge */
    VEdge( GEdge *prototype);
    
    /** Returns true if edge looks vertical in general */
    bool isVertical() const;
    /** Returns true if edge looks horizontal in general */
    bool isHorizontal() const;
    
    /** Get end node that is in 'up' direction, returns NULL if edge is horizontal */
    GNode *nodeUp() const;
    /** Get end node that is in 'down' direction, returns NULL if edge is horizontal */
    GNode *nodeDown() const;
    /** Get end node that is in 'left' direction, returns NULL if edge is vertical */
    GNode *nodeLeft() const;
    /** Get end node that is in 'right' direction, returns NULL if edge is vertical */
    GNode *nodeRight() const;
    
    /** Get pred */
    inline GNode* pred() const;
    /** Get succ */
    inline GNode *succ() const;
private:    
    GNode *pred_priv;
    GNode *succ_priv;
};

/** Get pred */
GNode* VEdge::pred() const
{
    return pred_priv;
}
/** Get succ */
GNode *VEdge::succ() const
{
    return succ_priv;
}

#endif /** VISIBLE_EDGE_H */