/**
 * @file: conf_utest.cpp 
 * Implementation of testing for lists 
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
#include "utils_iface.h"

/** Define the lists we use */
enum ListTypes
{
    LIST_ONE,
    LIST_TWO,
    LISTS_NUM
};
/** Derive class of linked objects */
class classA: public MListIface< classA, MListItem<LISTS_NUM>, LISTS_NUM>
{

};
/** Derive class from A */
class B: public MListIface< B, classA, LISTS_NUM>
{

};

/** MList testing */
static bool uTestMList()
{
    B *obj1 = new B();
    B *obj2 = new B();
    B *obj3 = new B();
    obj1->attach( LIST_ONE, obj2);
    obj1->attach( LIST_TWO, obj3);
    ASSERT( areEqP( obj1->next( LIST_ONE), obj2));
    ASSERT( areEqP( obj1->next( LIST_TWO), obj3));
    ASSERT( isNullP( obj1->prev( LIST_ONE)));
    ASSERT( isNullP( obj1->prev( LIST_TWO)));
    ASSERT( areEqP( obj2->prev( LIST_ONE), obj1));
    ASSERT( areEqP( obj3->prev( LIST_TWO), obj1));
    ASSERT( isNullP( obj3->prev( LIST_ONE)));
    ASSERT( isNullP( obj2->prev( LIST_TWO)));
    ASSERT( isNullP( obj2->next( LIST_ONE)));
    ASSERT( isNullP( obj3->next( LIST_TWO)));
    obj2->detachAll();
    delete obj2;
    delete obj1;
    delete obj3;
    return true;
}

/**
 * Test list classes operation
 */
bool uTestList()
{
    if ( !uTestMList())
        return false;

    return true;
}
