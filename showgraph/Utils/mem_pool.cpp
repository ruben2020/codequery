/**
 * @file: mem_pool.cpp 
 * Implementation of memory pool
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Mem;

/**
 * Default operator 'new' is disabled
 */
void *
PoolObj::operator new( size_t size)
{
    ASSERT( 0);
#ifdef _MSC_VER
    return NULL;
#endif
}
/**
 * Default operator 'delete' is disabled
 */
void 
PoolObj::operator delete( void *ptr)
{
    ASSERT( 0);
}
/**
 * Default operator 'new[]' is disabled
 */
void *
PoolObj::operator new[]( size_t size)
{
    ASSERT( 0);
#ifdef _MSC_VER
    return NULL;
#endif
}
/**
 * Default operator 'delete[]' is disabled
 */
void 
PoolObj::operator delete[]( void *ptr)
{
    ASSERT( 0);
}
