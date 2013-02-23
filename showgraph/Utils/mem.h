/**
 * @file: mem.h 
 * Some experiments with memory manager
 * @defgroup Mem Memory Manager
 *
 * Implementation of memory manager, In Process of Design&Implementation.
 *
 * Memory manager should solve the following memory-related problems
 * - Dangling pointers. Solution: reference counting via smart pointers
 * - Memory leaks. Solution: Various checks in pools and in pointers
 * - Fragmentation. Solution: Fixed-sized chunks used in pools
 * - Poor locality. Solution: not clear yet :(
 * @ingroup Utils
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#define MEM_H

#undef CHECK_CHUNKS
#undef CHECK_ENTRY
#undef USE_REF_COUNTERS
#undef USE_MEM_EVENTS
#undef CHECK_DELETE

#ifdef _DEBUG
#  define CHECK_CHUNKS
#  define CHECK_ENTRY
#  define USE_REF_COUNTERS
#  define USE_MEM_EVENTS
#  define CHECK_DELETE
#endif

#include <QtGlobal>

/**
 * Debug assert in memory manager
 * @ingroup Mem
 */
#if !defined(MEM_ASSERTD)
#    define MEM_ASSERTD(cond, what) ASSERT_XD(cond, "Memory manager", what)
#endif

/**
 * Namespace for memory-related routines
 * @ingroup Mem
 */
namespace Mem
{
    /* Class pool predeclaration */
    class Pool;
}



/**
 * Low level functinality for Mem package 
 * @defgroup MemImpl Memory Manager Low Level
 * @ingroup Mem
 */
namespace MemImpl
{
    /* Predeclaration on MemInfo */
    class MemInfo;
    /* Predeclaration of mem entry class */
    template < class Data> class Entry;
    /* Predeclaration of chunk class */
    template < class Data> class Chunk;
    /**
     * Position in chunk type
     * @ingroup MemImpl
     */
    typedef quint8 ChunkPos;
    /** 
     * Max number of entries in chunk
     * @ingroup MemImpl
     */
#ifndef MEM_SMALL_CHUNKS
    const quint8 MAX_CHUNK_ENTRIES_NUM = ( quint8)( -1);
#else
    const quint8 MAX_CHUNK_ENTRIES_NUM = 2;
#endif
    /**
     * 'NULL' equivalent for ChunkPos
     * @ingroup MemImpl
     */
    const ChunkPos UNDEF_POS = MAX_CHUNK_ENTRIES_NUM; 
};

namespace Mem
{
    /**
     * Singleton for memory manager
     * @ingroup MemImpl
     */
    typedef Single< MemImpl::MemInfo> MemMgr;
};

#include <stdlib.h>
#include "mem_mgr.h"        /** Memory manager */
#include "mem_ref.h"        /** Memory reference */
#include "mem_obj.h"        /** Memory object base class */
#include "mem_chunk.h"      /** Memory chunk class */
#include "mem_pool.h"       /** Memory pool */
#include "mem_entry.h"      /** Memory entry class */
#include "mem_fixed_pool.h" /** Memory pool */

#endif /* MEM_H */
