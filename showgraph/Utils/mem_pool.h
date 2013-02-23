/**
 * @file: mem_pool.h 
 * Implementation of memory pool
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_POOL_H
#define MEM_POOL_H

namespace Mem
{
    /**
     * Type for  memory entry size  
     * @ingroup Mem
     */
    typedef quint32 EntrySize;

    /** 
     * Type for number of memory entries 
     * @ingroup Mem
     */
    typedef quint32 EntryNum;

    /**
     * Pool types
     * @ingroup Mem
     */
    enum PoolType
    {
        /** Fixed-size entry pool */
        POOL_FIXED,
        /** Float-size entry pool */
        POOL_FLOAT,
        /** Number of types */
        POOL_TYPES_NUM
    };

    /**
     * Base class for memory pools
     *
     * @ingroup Mem
     */
    class Pool
    {
    public:
        /** Allocate new memory block */
        virtual void* allocate( size_t size) = 0;
        /** Free memory block */
        virtual void deallocate( void *ptr) = 0;
        /** Functionality of 'operator delete' for pooled objects */
        virtual void destroy( void *ptr) = 0;
        /** Destructor */
        virtual ~Pool(){};
    };

    /**
     * Base class for all objects allocated in pools
     *
     * @ingroup Mem
     */    
    class PoolObj
    {
#ifdef CHECK_DELETE
        bool to_be_deleted;
#endif  
        /** Copy constructor disabled*/
        PoolObj( PoolObj &obj){};
        /** Assignment disabled */
        PoolObj& operator = ( const PoolObj& obj){ return *this;};

        public:
#ifdef CHECK_DELETE
        /** Default constructor */
        PoolObj(): to_be_deleted( false){};

        /** Mark for deletion */
        inline void toBeDeleted()
        {
            MEM_ASSERTD( !to_be_deleted, "Tried to mark object for deletion more than once");
            to_be_deleted = true;
        }
#else
        /** Default constructor */
        inline PoolObj(){};
#endif
        /** Default operator 'new' is disabled */
        void *operator new ( size_t size);
        /** Default operator 'delete' is disabled */
        void operator delete( void *ptr);
        
        /** Default operator 'new' is disabled */
        void *operator new[] ( size_t size);
        /** Default operator 'delete' is disabled */
        void operator delete[] ( void *ptr);
        
        /** Placement new */
        inline void *operator new ( size_t size, Pool* pool);
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void operator delete( void *ptr, Pool* pool);
        /** Destructor is to be called by 'destroy' routine of pool class */
        virtual ~PoolObj()
        {
#ifdef CHECK_DELETE
            MEM_ASSERTD( to_be_deleted, "Deleted pool object not through pool interface. Probably operator delete used.");
#endif        
        }
    };

    /**
     * Placement new
     */
    inline void *
    PoolObj::operator new ( size_t size, Pool* pool)
    {
        return pool->allocate( size);
    }
    /**
     * Operator 'delete' corresponding to placement new
     * WARNING: Compiler won't call this for deletion. 
     *          It is needed for freeing memory in case of exceptions in constructor
     */
    inline void
    PoolObj::operator delete( void *ptr, Pool* pool)
    {
        pool->deallocate( ptr);
    }
}; /* namespace Mem */
#endif /* MEM_POOL_H */