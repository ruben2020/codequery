/**
 * @file: mem_pool.h 
 * Implementation of memory pool
 */
/*
 * Utils library in Showgraph tool
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
