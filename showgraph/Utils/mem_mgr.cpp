/**
 * @file: mem_mgr.cpp 
 * Implementation of memory manager
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "mem.h"

using namespace MemImpl;

MemInfo::MemInfo()
{
    alloc_counter = 1;
    dealloc_counter = 1;
}

/** Place for breakpoint on event number */
void MemInfo::allocReg( MemEventId n)
{

}

/** Place for breakpoint on event number */
void MemInfo::deallocReg( MemEventId n)
{

}

/** Registers the allocation event and returns its number */
MemEventId MemInfo::allocEvent()
{
    /** Increase counter */
    alloc_counter++;
    allocReg( alloc_counter);
    return alloc_counter;
}

/** Registers the deallocation event and returns its number */
MemEventId MemInfo::deallocEvent()
{
    /** Increase counter */
    dealloc_counter++;
    deallocReg( dealloc_counter);
    return dealloc_counter;
}
