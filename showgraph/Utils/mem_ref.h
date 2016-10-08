/**
 * @file: mem_ref.h 
 * Implementation of memory reference template
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

#ifndef MEM_REF_H
#define MEM_REF_H

namespace Mem
{
   /**
     * Memory reference template
     * @ingroup Mem
     */
    template < class RefObj> class Ref
    {
#ifdef USE_REF_COUNTERS

#endif
	private:
		RefObj *ptr;
	public:
		/** Default constructor */
		Ref(): ptr( NULL){};
		/** Copy constructor !!! NO uTEST !!! */
		Ref( const Ref< RefObj>& orig): ptr( orig.ptr)
		{
#ifdef USE_REF_COUNTERS
			ptr->incRefCount();
#endif
		}
		/** Assignement operator overloading */
		Ref< RefObj> & operator=( const Ref< RefObj>& orig)
		{
			ptr = orig.ptr;
#ifdef USE_REF_COUNTERS
			ptr->incRefCount();
#endif
			return *this;
		}
	    /**
		 * Constructor from pointer
		 * Used in Ref ref = new Obj(...) initialization
		 * !!! No uTEST !!!
		 */
		Ref( RefObj* p): ptr( p)
		{
#ifdef USE_REF_COUNTERS
			if ( ptr != 0)
				ptr->incRefCount();
#endif	
		}
		/**
		 * Assignement of pointer
		 * Used in Ref ref; ref = new Obj(...) expression
		 */
		Ref< RefObj> & operator=( RefObj* p)
		{
#ifdef USE_REF_COUNTERS
			/** Decrement object's ref count */
			if ( ptr != 0)
				ptr->decRefCount();
#endif	
			/** Assign a new pointer */
			ptr = p;
#ifdef USE_REF_COUNTERS
			/** Increment ref count */
			if ( ptr != 0)
				ptr->incRefCount();
#endif	
			return *this;
		}
		/** Destructor */
		~Ref()
		{
#ifdef USE_REF_COUNTERS
			if ( ptr != 0)
				ptr->decRefCount();
#endif
		}
		/** Member access operator */
		inline RefObj* operator->()
		{
			return ptr;
		}
		/** Equals operator */
		inline bool operator == ( Ref< RefObj> &r)
		{
			return ptr == r.ptr;
		}
		/** Conversion to boolean */
		inline operator bool()
		{
			return ptr != NULL;
		}
		/**
		 * Conversion to Pointer
		 * For using ref as delete operator argument.
		 * Use in other expressions is prohibited. Unfortunatelly this is not ( can't be?) enforced.
		 */
		inline operator RefObj*()
		{
                        RefObj *ret_val = ptr;
			assertd( ret_val != NULL);
			/** Decrement object's ref count */
			if ( ptr != 0)
			{
#ifdef USE_REF_COUNTERS
				ptr->decRefCount();	
#endif
				ptr = 0;
			}
			return ret_val;
		}
    };
};
#endif /* MEM_REF_H */