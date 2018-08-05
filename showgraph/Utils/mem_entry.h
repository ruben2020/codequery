/**
 * @file: mem_entry.h 
 * Implementation of memory entry
 */
/*
 * Utils/Mem library in Showgraph tool
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
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_ENTRY_H
#define MEM_ENTRY_H

namespace MemImpl
{
    /**
     * Entry in memory that holds user data
     *
     * @ingroup MemImpl
     */
    template < class Data> class Entry: private Data
    {
    public:
        /** Get position */
        inline ChunkPos pos() const;
        /** Get position of next free chunk */
        inline ChunkPos nextFree() const;
        /** Set position */
        inline void setPos( ChunkPos pos);
        /** Set position of next free chunk */
        inline void setNextFree( ChunkPos next);
    private:
        /** Classes fixed pool and chunk should have access to data and constructors */
        friend class Chunk<Data>;

        /** Own position of this entry in chunk */
        ChunkPos my_pos;
        /** Position of next free entry in chunk */
        ChunkPos next_free_pos;

#ifdef CHECK_ENTRY        
        /** Debug info: entry status */
        bool is_busy;
#endif

#ifdef USE_MEM_EVENTS        
        /** Debug info: alloc event */
        MemEventId alloc_event;
        /** Debug info: dealloc event */
        MemEventId dealloc_event;
#endif  

        /** Private constructor to prevent direct creation of such objects */
        Entry();
        /** Private destructor */
        ~Entry();
    };

    /**
     * Private constructor to prevent direct creation of such objects
     */
    template< class Data>
    Entry< Data>::Entry()
    {
        assertd( 0);
    }
    /**
     * Private destructor
     */
    template< class Data>
    Entry< Data>::~Entry()
    {
        assertd( 0);
    }
    /** Get position */
    template< class Data>
    ChunkPos 
    Entry< Data>::pos() const
    {
        return my_pos;
    }
    /** Get position of next free chunk */
    template< class Data>
    ChunkPos
    Entry< Data>::nextFree() const
    {
        return next_free_pos;
    }
    /** Set position */
    template< class Data>
    void
    Entry< Data>::setPos( ChunkPos pos)
    {
        my_pos = pos;
    }
    /** Set position of next free chunk */
    template< class Data>
    void
    Entry< Data>::setNextFree( ChunkPos next)
    {
        next_free_pos = next;
    }
};

#endif /* MEM_ENTRY_H */
