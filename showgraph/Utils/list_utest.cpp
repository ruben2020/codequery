/**
 * @file: conf_utest.cpp 
 * Implementation of testing for lists 
 */
/*
 * Copyright (C) 2009  Boris Shurygin
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
