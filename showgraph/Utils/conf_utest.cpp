/**
 * @file: conf_utest.cpp 
 * Implementation of testing of configuration 
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"

/**
 * Test configuration and options parsing
 */
bool uTestConf()
{
    Conf *conf = new Conf();
    
    /** Create some example options */
    conf->addOption( new Option( OPT_STRING, "o", "output", "output file name( string option example)"));
    conf->addOption( new Option( OPT_BOOL, "b", "boolean", "boolean option example"));
    conf->addOption( new Option( OPT_INT, "i", "integer", "integer option example"));
    conf->addOption( new Option( OPT_FLOAT, "f", "float", "float option example"));
    conf->printOpts(); // Print them to console

    /** Check that created options can be accessed */
    assert( isNullP( conf->option( "file")));
    assert( isNotNullP( conf->option("output")));
    assert( isNotNullP( conf->shortOption( "o")));
    assert( isNullP( conf->shortOption( "output")));
    assert( isNotNullP( conf->longOption("output")));

    /** Create array of arguments */
    char *args[ 8];
    args[ 0] = (char*)"string";// treated
    args[ 1] = (char*)"--output";
    args[ 2] = (char*)"file";
    args[ 3] = (char*)"-a";
    args[ 4] = (char*)"--b";
    args[ 5] = (char*)"-b";
    args[ 6] = (char*)"-i";
    args[ 7] = (char*)"80";

    /** Read arguments from array and parse them */
    conf->readArgs( 8, args);

    /** Check options values */
    assert( conf->unknownOptsNum() == 2); // Check number of unknown arguments
    assert( !(conf->option( "output")->string().compare("file")));
    Option *int_opt = conf->option( "integer");
    assert( int_opt->isDefined());
    assert( int_opt->intVal()== 80);
    assert( conf->option( "b")->isSet());
    
    delete conf;
    return true;
}
