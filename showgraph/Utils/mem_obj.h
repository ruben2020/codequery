/**
 * @file: mem_obj.h 
 * Implementation of memory object base class
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
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
		inline Obj() 
#ifdef USE_REF_COUNTERS
		:ref_count( 0)
#endif
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