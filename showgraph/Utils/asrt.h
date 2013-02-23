/**
 * @file: asrt.h 
 * Assertion related routines of Utils library for ShowGraph
 * @defgroup Asserts Assertions
 * @brief Assertion routines/macros
 * @ingroup Utils
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef ASRT_H
#define ASRT_H

/**
 * Assert macro that works only in debug version
 * @ingroup Asserts
 */
#if !defined(ASSERTD)
#  ifdef _DEBUG
#    define ASSERTD(cond) Q_ASSERT(cond)
#  else
#    define ASSERTD(cond)
#  endif
#endif

/**
 * @brief Assert macro with description that works only in debug version
 * @ingroup Asserts
 * @param cond The condition
 * @param where Description of functionality where failure occured
 * @param what Error message explaining what happened
 *
 * Assertion that in case of failure provides additional info about what happened
 */
#if !defined(ASSERT_XD)
#  ifdef _DEBUG
#    define ASSERT_XD(cond, where, what) Q_ASSERT_X(cond, where, what)
#  else
#    define ASSERT_XD(cond, where, what)
#  endif
#endif

/** 
 * Simple assert macro
 * @ingroup Asserts
 */
#if !defined(ASSERT)
#  define ASSERT(cond) Q_ASSERT(cond)
#endif

/**
 * Generic assertion routine template
 * @ingroup Asserts
 */
template<class Excpt> inline void assert( bool assertion)
{
    if ( !assertion)
    {
        throw Excpt();
    }
}

/**
 * Assertion routine template parameterized with thrown exception type
 * @ingroup Asserts
 */
template<class Excpt> inline void assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}

/**
 * Simple assertion routine 
 * @ingroup Asserts
 */
inline void assert( bool asrt)
{
    assert< int>( asrt);
}

/**
 * Assert that works only in debug version
 * @ingroup Asserts
 */
inline void assertd( bool asrt)
{
#ifdef _DEBUG
    assert< int>( asrt);
#endif
}

#endif /* ASRT_H */
