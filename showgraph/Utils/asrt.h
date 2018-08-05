/**
 * @file: asrt.h 
 * Assertion related routines of Utils library for ShowGraph
 * @defgroup Asserts Assertions
 * @brief Assertion routines/macros
 * @ingroup Utils
 */
/*
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

#if 0
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
#endif

/**
 * Assert that works only in debug version
 * @ingroup Asserts
 */
inline void assertd( bool asrt)
{
#ifdef _DEBUG
    //assert< int>( asrt);
#endif
}

#endif /* ASRT_H */
