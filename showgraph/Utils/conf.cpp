/**
 * @file: conf.cpp 
 * Implementation of configuration tools
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
#include <QRegExp>

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define QTENDL     Qt::endl
#else
#define QTENDL     endl
#endif

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
           << descr << QTENDL;
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
                err << "No such option " << name << QTENDL;
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
                err << "No such option " << name << QTENDL;
                unknown_options.push_back( name);
            }
        } else
        {
            out("WTF");
            /* Is not an option specifier */
            err << "Unrecognized argument " << curr << QTENDL;
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
