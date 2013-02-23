/**
 * @file: utils_utest.cpp 
 * Implementation of testing of utils
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"


/** Sample class */
class classA
{
private:
    /** private constructors, assignment and destructor */
    classA(){};
    classA( const classA&){};
    classA& operator =( const classA&){};
    ~classA(){};
    /** Needed for singleton creation */
    friend class Single< classA>;
};

/** Typedef for classA encapsulated in singleton */
typedef Single< classA> SingleA;

/**
 * Test simple singleton
 */
bool uTestSingle()
{
    SingleA::init();

    classA* a1 = SingleA::instance();
    classA* a2 = SingleA::instance();
    ASSERT( areEqP( a1, a2));
    SingleA::deinit();
    return true;
}

/**
 * Test Utils package
 */
bool uTestUtils()
{
    /** test singleton */
    if ( !uTestSingle())
        return false;

    /** Tets list classes */
    if ( !uTestList())
        return false;

    /** Test memory management */
    if ( !uTestMem())
		return false;

    /** Test configuration classes functionality */
    if ( !uTestConf())
    	return false;
	return true;
}
