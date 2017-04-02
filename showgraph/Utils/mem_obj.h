/**
 * @file: mem_obj.h 
 * Implementation of memory object base class
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
#	error
#endif

#ifndef MEM_OBJ_H
#define MEM_OBJ_H

/**
 * Namespace for memory manager
 * @ingroup Mem
 */
namespace Mem
{
    /** Type of reference count */
	typedef unsigned int RefNumber;

	/**
     * Base class for all memory-managed objects
     * @ingroup Mem
     */
    class Obj
    {
    private:
        /** Pointer to pool */
        Pool *_pool;
        
#ifdef USE_REF_COUNTERS
	private:
		/** Counter for references */
		RefNumber ref_count; /* Additional memory used, the overhead of counted references */
	public:	
		/** Get the number of references */
		inline RefNumber refCount() const
		{
			return ref_count;
		}
		/** Increase reference count */
		inline void incRefCount()
		{
			ref_count++;
		}
		/** Decrease reference count */
		inline void decRefCount()
		{
			assertd( ref_count > 0);
			ref_count--;
		}
#endif		
	public:
        /** Get pointer to pool */
        inline Pool* pool() const
        {
            return _pool;
        }

		/** Constructor */
		inline Obj():
#ifdef USE_REF_COUNTERS
		ref_count( 0),
#endif
		_pool(NULL)
		{
		
		}
		/** Destructor */
		~Obj()
		{
#ifdef USE_REF_COUNTERS
			assertd( ref_count == 0);
#endif
		}
    };
};

#endif /** MEM_OBJ_H */
