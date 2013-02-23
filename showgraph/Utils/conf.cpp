/**
 * @file: conf.cpp 
 * Implementation of configuration tools
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include <QRegExp>

/** 
 * namespaces import
 */
using namespace Utils;

/** Option's print routine */
void
Option::print( QTextStream &stream)
{
    //QTextStream stream( stdout);
    
    stream << "-" << short_name << ", "
           << "--" << long_name << "    "
           << descr << endl;
}

/** Default constructor of configuration */
Conf::Conf(): short_opts(), long_opts(), unknown_options()
{
#ifdef _DEBUG
    /* Debug options */

#endif
    /* General purpose options */
}

/** Print options */
void Conf::printOpts()
{
    QTextStream stream( stdout);
    
    foreach( Option *opt, short_opts)
    {
        opt->print( stream);
    }
}

/** Print value defaults */
void Conf::printDefaults()
{

}

/** Read input arguments */
void Conf::readArgs( int argc, char** argv)
{
    app_name = QString( argv[ 0]);

    QTextStream err( stderr);
    
    for ( int i = 1; i < argc; i++)
    {
        QString curr( argv[ i]);
        QRegExp short_rx("^-([^-]+)");
        QRegExp long_rx("^--([^-]+)");
        Option *opt = NULL;
        if ( short_rx.indexIn( curr) != -1 )
        {
            /* We look for expression among short option names */
            QString name = short_rx.cap( 1);
            
            if ( short_opts.find( name) != short_opts.end())
            {
                opt = short_opts[ name];
            } else
            {
                err << "No such option " << name << endl;
                unknown_options.push_back( name);
            }
        } else if (  long_rx.indexIn( curr) != -1)
        {
            /* We look for expression among long option names */
            QString name = long_rx.cap( 1);
            if ( long_opts.find( name) != long_opts.end())
            {
                opt = long_opts[ name];
            } else
            {
                err << "No such option " << name << endl;
                unknown_options.push_back( name);
            }
        } else
        {
            out("WTF");
            /* Is not an option specifier */
            err << "Unrecognized argument " << curr << endl;
            unknown_options.push_back( curr);
        }
        if ( isNotNullP( opt))
        {
            OptType tp = opt->type();
            opt->setDefined(); // option is defined now
            switch( tp)
            {
                case OPT_BOOL:
                    /* For bool options we expect argument only if its default value is 'true' */
                    if ( opt->defBoolVal())
                    {
                        i++; // advance in argument array
                        QString val( argv[ i]);
                        opt->setBoolVal( val.toInt());
                    } else /* However most bool options have default value of 'false' */
                    {
                        opt->setBoolVal( true);
                    }
                    break;
                case OPT_INT:
                {
                    i++; // advance in argument array
                    QString val( argv[ i]);
                    opt->setIntVal( val.toInt());
                    break;
                }
                case OPT_FLOAT:
                {
                    i++; // advance in argument array
                    QString val( argv[ i]);
                    opt->setFloatVal( val.toFloat());
                    break;
                }
                case OPT_STRING:
                {
                    i++; // advance in argument array
                    QString val( argv[ i]);
                    opt->setStringVal( val);
                    break;
                }
                default:
                    break;
            }
        }
    }
}