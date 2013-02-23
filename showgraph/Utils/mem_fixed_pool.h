/**
 * @file: mem_fixed_pool.h 
 * Implementation of memory pool with fixed entry size
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_FIXED_POOL_H
#define MEM_FIXED_POOL_H


namespace Mem
{
    /**
     * @brief Memory pool with fixed-size entries.
     * @ingroup Mem
     * @param Data Type of objects stored in pool.
     *
     * @details  
     * A <em>Fixed pool</em> is a pool that creates entries of the same size which
     * is defined by template parameter. It uses the knowledge of the entry size for
     * optimization of allocation/deallocation process and for simplifying the internal
     * bookkeeping. This simplification significantly speeds up pool's operation.
     *
     * Internal implementation of a fixed pool is based on allocating memory in so-called
     * <em>chunks</em>. A <em>chunk</em> is a continuous block of memory which holds
     * a number of entries and is allocated with one system call. The allocated memory
     * is managed by the pool and released to system when pool doesn't need it. This
     * policy effectively avoids calling malloc() and free() routines for every entry
     * and prevents fragmentation. FixedPool internal implementattion trades off compactness
     * for quickness and uses additional info for every entry. Every entry keeps:
     *  - Its own number. To calculate pointer to chunk info from pointer to entry.
     *  - Number of next free entry in this chunk. For speeding up the allocation.
     *  - Debug info like allocation/deallocation event number or ``free'' flag for checking 
     *    double deallocation.
     *
     * The chunks are organized in doubly-liked list to have constant time for chunk 
     * creation and destruction. Free chunks are also linked in list to speed up the 
     * allocation. On allocation request pool simply gets its first free chunk and 
     * returns pointer to its first entry. On deallocation, pointer to chunk is calculated
     * from pointer to entry using its own number and entry is simply connected to the
     * head of free entry list in the chunk. The chunk itself is enrolled in the list of
     * free chunks if it isn't already there. If the chunk is completely empty after 
     * this deallocation it may be deleted if there are other free chunks present. Both
     * allocation and deallocation demand constant time unless we have to allocate a new
     * chunk or free existing empty one.
     *
     * To decrease memory usage overhead the numbers in entries are effectively one byte long.
     * Thats two byte per entry overhead if we don't align entries on 8, 16, 32 or 64 bytes.
     * Thus memory overhead is significant if we store small objects in such a pool but for 
     * a list unit this overhead is about 25% and for graph's node or edge it is 6.2%.
     *
     * When project is built in debug mode the pools keep track of every entry's allocation
     * and deallocation ID. This allows programmer to put in a breakpoint if a memory leak
     * or double-delete occurred. The conditional breakpoint should be placed in
     * MemInfo::allocReg( n) or MemInfo::deallocReg( n) where 'n' should be the ID that
     * can be obtained from suspicious entry.
     */
    template < class Data> 
    class FixedPool: public Pool
    {
        static const size_t CHUNK_SIZE = sizeof( MemImpl::Chunk< Data>) 
            + sizeof( MemImpl::Entry< Data>) * MemImpl::MAX_CHUNK_ENTRIES_NUM;
    public:
        /** Create fixed pool with default parameters */
        FixedPool();
        
        /** Destroy the pool */
        ~FixedPool();
                
        /** Allocate new memory block */
        void* allocate( size_t size);
        /** Free memory block */
        void deallocate( void *ptr);
        /** Functionality of 'operator delete' for pooled objects */
        void destroy( void *ptr); 
#ifdef _DEBUG
        /** Get first busy chunk */
        inline MemImpl::Chunk< Data> *firstBusyChunk();
#endif
    private:        
        /** Number of used entries */
        EntryNum entry_count;
        /** First chunk */
        MemImpl::Chunk< Data> *first_chunk;
        /** First free chunk */
        MemImpl::Chunk< Data> *free_chunk;

        /* Internal routines */
        
        /** Allocate one chunk */
        inline MemImpl::Chunk< Data> *allocateChunk();
        /** Deallocate one chunk */
        inline void deallocateChunk( MemImpl::Chunk< Data> *chunk);
        /** Get pointer to chunk from pointer to entry */
        inline MemImpl::Chunk< Data> *entryChunk( MemImpl::Entry< Data> *e);
    };

    /** Create fixed pool with default parameters */
    template < class Data> 
    FixedPool<Data>::FixedPool(): 
        entry_count( 0),
        first_chunk( NULL),
        free_chunk( NULL)
    {

    }

    /** Destroy the pool */
    template < class Data> 
    FixedPool< Data>::~FixedPool()
    {
        /** Deallocated cached chunks */
        while ( isNotNullP( first_chunk))
        {
            deallocateChunk( first_chunk);
        }
        /** Check that all entries are freed */
        MEM_ASSERTD( entry_count == 0, "Trying to delete non-empty pool");
    }
#ifdef _DEBUG
    /** Get first busy chunk */
    template < class Data> 
    MemImpl::Chunk< Data> *
    FixedPool< Data>::firstBusyChunk()
    {
        MemImpl::Chunk< Data> *chunk = first_chunk;
        while ( isNotNullP( chunk))
        {
            if ( !chunk->isEmpty())
                return chunk;
            chunk = chunk->next( MemImpl::CHUNK_LIST_ALL);
        }
        return NULL;
    }
#endif
    /** Allocate one chunk */
    template < class Data> 
    MemImpl::Chunk< Data> *
    FixedPool< Data>::allocateChunk()
    {
        /* We should only allocate chunk if there are no free chunks left */
        MEM_ASSERTD( isNullP( free_chunk ), "Tried to deallocate chunk while there is a free chunk");
        
        /* Allocate memory for chunk */
        void *chunk_mem = 
              ( MemImpl::Chunk< Data> *) new quint8[ CHUNK_SIZE];
        MemImpl::Chunk< Data> * chunk = new ( chunk_mem) MemImpl::Chunk< Data>();

        /* Add this chunk to pool */
        chunk->attach( MemImpl::CHUNK_LIST_ALL, first_chunk);
        chunk->attach( MemImpl::CHUNK_LIST_FREE, free_chunk);
        first_chunk = chunk;
        free_chunk = chunk;
        
#ifdef CHECK_CHUNKS
        chunk->pool = ( void *)this;
#endif
        return chunk;
    }
    
    /** Deallocate one chunk */
    template < class Data> 
    void
    FixedPool<Data>::deallocateChunk( MemImpl::Chunk< Data> *chunk)
    {
#ifdef CHECK_CHUNKS
        if ( !chunk->isEmpty())
        {
            MEM_ASSERTD( isNotNullP( chunk->firstBusyEntry()),
                         "Can't get first busy entry of non-empty chunk");
            MEM_ASSERTD( 0, "Deallocated chunk is not empty. Check allocation ID of some busy entry");
        }
        MEM_ASSERTD( areEqP( chunk->pool, this), "Deallocated chunk does not belong to this pool");
#endif
        if ( areEqP( first_chunk, chunk))
        {
            first_chunk = chunk->next( MemImpl::CHUNK_LIST_ALL);
        }
        chunk->~Chunk();
        delete[] (quint8 *)chunk;
    }

    /* Calculate pointer to chunk from pointer to entry */
    template < class Data> 
    MemImpl::Chunk< Data> *
    FixedPool<Data>::entryChunk( MemImpl::Entry< Data> *e)
    {
        MemImpl::ChunkPos e_pos = e->pos();
        quint8 *ptr = ( quint8 *) e;
        ptr = ptr - sizeof( MemImpl::Entry< Data>) * e_pos - sizeof ( MemImpl::Chunk< Data>);
        return (MemImpl::Chunk< Data> *) ptr;
    }

    /* Allocate new memory block */
    template < class Data> 
    void* 
    FixedPool<Data>::allocate( size_t size)
    {
        MEM_ASSERTD( sizeof( Data) == size,
                     "Allocation size doesn't match FixedPool's template parameter size");
        void *ptr = NULL;
        /* If we don't have a free chunk */
        if ( isNullP( free_chunk))
        {
            /* We need to create new chunk */
            allocateChunk();
        } 
        MEM_ASSERTD( free_chunk->isFree(), "Pool's first free chunk is not free");
        /* allocate one entry */
        ptr = ( void *)free_chunk->allocateEntry();
        /* if no more entries left */
        if ( !free_chunk->isFree())
        {
            MemImpl::Chunk< Data> *chunk = free_chunk;
            free_chunk = chunk->next( MemImpl::CHUNK_LIST_FREE);
            chunk->detach( MemImpl::CHUNK_LIST_FREE);
        }
        entry_count++;
        return ptr;
    }

    /** Free memory block */
    template < class Data> 
    void
    FixedPool<Data>::deallocate( void *ptr)
    {
        /* 1. Check pointer */
        MEM_ASSERTD( isNotNullP( ptr), "Deallocation tried on NULL pointer");
        
        /* 2. Check entry count */
        MEM_ASSERTD( entry_count > 0, "Trying deallocate entry of an empty pool"); 

        MemImpl::Entry< Data> *e =(MemImpl::Entry< Data> *) ptr;
        
        /* 3. Get chunk of the deallocated entry */
        MemImpl::Chunk< Data> *chunk = entryChunk( e);

#ifdef CHECK_CHUNKS
        /* 4. Check that we are deleting entry from this pool */
        MEM_ASSERTD( areEqP( this, chunk->pool), "Trying deallocate entry from a wrong pool"); 
#endif
        /*
         * 5. If chunk is free already - it must be in free list 
         * no need to add it again
         */
        bool add_to_free_list = !chunk->isFree();

        /* 6. Free entry in chunk */
        chunk->deallocateEntry( e);
        
        /*
         * 7. If this chunk is not the same as the current 'free chunk' 
         *     add it to free list or deallocate it if it is empty
         */
        if ( areNotEqP( chunk, free_chunk))
        {
            if ( add_to_free_list)
            {
                /* Add chunk to free list if it is not already there */
                chunk->attach( MemImpl::CHUNK_LIST_FREE, free_chunk);
                /* Deallocate previous free chunk if it is empty */
                if ( isNotNullP( free_chunk) && free_chunk->isEmpty())
                {
                    deallocateChunk( free_chunk);
                }
                free_chunk = chunk;
            } else if ( chunk->isEmpty())
            {
                /* Deallocate this chunk if it is empty and not the first free chunk */
                deallocateChunk( chunk);
            }
        }
        entry_count--;
    }
    /** Functionality of 'operator delete' for pooled objects */
    template < class Data> 
    void
    FixedPool<Data>::destroy( void *ptr)
    {
        /* 1. Check null pointer( in DEBUG mode) */
        MEM_ASSERTD( isNotNullP( ptr), "Destruction tried on NULL pointer");

        Data *data_p = static_cast< Data *>( ptr);
        
#ifdef CHECK_DELETE
        /* 2. Mark for deletion */
        data_p->toBeDeleted();
#endif
        /* 3. Call destructor */
        data_p->~Data();
        
        /* 4. Free memory */
        deallocate( ptr);
    }
}; /* namespace Mem */
#endif /* MEM_FIXED_POOL_H */