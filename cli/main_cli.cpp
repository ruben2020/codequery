
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "small_lib.h"
#include "getopt2.h"
#include "sqlquery.h"
#include "swver.h"


void printhelp(const char* str)
{
	printf("Usage:\n");
	printf("%s [-s <sqdbfile> [-p <n>] [-g] [-t <term>] [-l <len>] -[e|f] [-u] [-b <path>]]  [-d] [-v] [-h]\n\n", str);
	printf("options:\n");
	printf("  -s : CodeQuery sqlite3 db file path\n");
	printf("  -p : parameter is a number denoted by n\n");
	printf("       default n = 1 (Symbol)\n");
	printf("  -g : Create GraphViz DOT output instead of search results\n");
	printf("       Applicable only for function or macro def(n=2) and\n");
	printf("       class or struct (n=3)\n");
	printf("  -t : search term without spaces\n");
	printf("       if Exact Match is switched off, wild card\n");
	printf("       searches are possible. Use * and ?\n");
	printf("  -l : limited line length for truncated source code preview text\n");
	printf("       Accepted range: 0 - 800 (use 0 for limitless)\n");
	printf("       By default it is 80\n");
	printf("  -e : Exact Match switched ON \n");
	printf("       Case-sensitive\n");
	printf("  -f : Exact Match switched OFF (fuzzy search)\n");
	printf("       Case-insensitive with wild card search (default)\n");
	printf("  -u : show full file path instead of file name\n");
	printf("  -b : filters the results by source file path term\n");
	printf("       For example, if the path term is \"test\" (without quotes),\n");
	printf("       then the results will be filtered by source files whose\n");
	printf("       path includes the term \"test\"\n");
	printf("  -d : debug\n");
	printf("  -v : version\n");
	printf("  -h : help\n\n");
	printf("The combinations possible are -s -t -e, -s -t -f -u\n");
	printf("The additional optional arguments are -d\n\n");
	printf("The possible values for n are:\n");
	printf("    1: Symbol (default)\n");
	printf("    2: Function or macro definition\n");
	printf("    3: Class or struct\n");
	printf("    4: Files including this file\n");
	printf("    5: Full file path\n");
	printf("    6: Functions calling this function\n");
	printf("    7: Functions called by this function\n");
	printf("    8: Calls of this function or macro\n");
	printf("    9: Members and methods of this class\n");
	printf("   10: Class which owns this member or method\n");
	printf("   11: Children of this class (inheritance)\n");
	printf("   12: Parent of this class (inheritance)\n");
	printf("   13: Functions or macros inside this file\n\n");
	printf("Example:\n%s -s myproject.db -p 6 -t read*file -f\n\n", str);
	printf("Example of using -g with GraphViz:\n");
	printf("%s -s myproject.db -p 2 -g -t read*file > functioncallgraph.dot\n", str);
	printf("dot -Tpng -ofunctioncallgraph.png functioncallgraph.dot\n\n");

}

void printlicense(void)
{
	printf(CODEQUERY_SW_VERSION);
	printf("\n");
	printf(CODEQUERY_SW_LICENSE);
}

bool fileexists(const char* fn)
{
	bool retval;
	FILE *fp;
	fp = fopen(fn, "r");
	retval = (fp != NULL);
	if (retval) fclose(fp);
	return retval;
}

void process_argwithopt(char* thisOpt, int option, bool& err, tStr& fnstr, bool filemustexist)
{
	if (thisOpt != NULL)
	{
		fnstr = thisOpt;
		if (filemustexist && (fileexists(fnstr.c_str()) == false))
		{
			printf("Error: File %s doesn't exist.\n", fnstr.c_str());
			err = true;
		}
	}
	else
	{
		printf("Error: -%c used without file option.\n", option);
		err = true;
	}
}

tStr limitcstr(int limitlen, tStr str)
{
	if (limitlen <= 0)
		return str;
	else
		return str.substr(0,limitlen);
}

int process_query(tStr sqfn, tStr term, tStr param, bool exact, 
		   bool full, bool debug, int limitlen, bool graph, tStr fpath)
{
	if ((sqfn.empty())||(term.empty())||(param.empty())) return 1;
	int retVal = 0;
	sqlquery sq;
	tStr lstr;
	sqlquery::en_filereadstatus filestatus = sq.open_dbfile(sqfn);
	tVecStr grpxml, grpdot;
	bool res = false;
	tStr errstr;
	switch (filestatus)
	{
		case sqlquery::sqlfileOK:
			break;
		case sqlquery::sqlfileOPENERROR:
			printf("Error: File %s open error!\n", sqfn.c_str());
			return 1;
		case sqlquery::sqlfileNOTCORRECTDB:
			printf("Error: File %s does not have correct database format!\n", sqfn.c_str());
			return 1;
		case sqlquery::sqlfileINCORRECTVER:
			printf("Error: File %s has an unsupported database version number!\n", sqfn.c_str());
			return 1;
		case sqlquery::sqlfileUNKNOWNERROR:
			printf("Error: Unknown Error!\n");
			return 1;
	}
	int intParam = atoi(param.c_str()) - 1;
	if ((intParam < 0) || (intParam >= sqlquery::sqlresultAUTOCOMPLETE))
	{
		printf("Error: Parameter is out of range!\n");
		return 1;	
	}
	sqlqueryresultlist resultlst;
	if (graph)
	{
		if (intParam == sqlquery::sqlresultFUNC_MACRO)
		{
			res = sq.search_funcgraph(term, exact, grpxml, grpdot, 1, &errstr);
		}
		else if (intParam == sqlquery::sqlresultCLASS_STRUCT)
		{
			res = sq.search_classinheritgraph(term, exact, grpxml, grpdot, &errstr);
		}
		else
		{
			printf("Error: -g only works with function or macro def(n=2) and\n");
			printf("       class or struct (n=3)\n");
			return 1;
		}
		if (res == false)
		{
			printf("Error: SQL Error! %s!\n", errstr.c_str());
			return 1;
		}
		else
		{
			for (auto it = grpdot.begin(); it != grpdot.end(); it++)
				printf("%s\n", it->c_str());
			return 0;
		}
	}
	resultlst = sq.search(term, (sqlquery::en_queryType) intParam, exact, fpath);
	if (resultlst.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		printf("Error: SQL Error! %s!\n", resultlst.sqlerrmsg.c_str());
		return 1;	
	}
	for(std::vector<sqlqueryresult>::iterator it = resultlst.resultlist.begin();
		it != resultlst.resultlist.end(); it++)
	{
		lstr = limitcstr(limitlen, it->linetext);
		switch(resultlst.result_type)
		{
			case sqlqueryresultlist::sqlresultFULL:
				printf("%s\t%s:%s\t%s\n", 
						it->symname.c_str(), 
						(full ? it->filepath.c_str() : it->filename.c_str()),
						it->linenum.c_str(),
						lstr.c_str());
				break;	
			case sqlqueryresultlist::sqlresultFILE_LINE:
				printf("%s:%s\t%s\n",
						(full ? it->filepath.c_str() : it->filename.c_str()),
						it->linenum.c_str(),
						lstr.c_str());
				break;	
			case sqlqueryresultlist::sqlresultFILE_ONLY:
				printf("%s\n", it->filepath.c_str());
				break;
			default:
				break;	
		}
	}
	return retVal;
}

int main(int argc, char *argv[])
{
	int c;
	bool bSqlite, bParam, bGraph, bTerm, bExact, bFull;
	bool bDebug, bVersion, bHelp, bError;
	int countExact = 0;
	int limitlen = 80;
	bSqlite = false;
	bParam = false;
	bGraph = false;
	bTerm = false;
	bExact = false;
	bFull = false;
	bDebug = false;
	bVersion = false;
	bHelp = (argc <= 1);
	bError = false;
	tStr sqfn, param = "1", term, fpath = "";

    while ((c = getopt2(argc, argv, "s:p:gt:l:efub:dvh")) != -1)
    {
		switch(c)
		{
			case 'v':
				bVersion = true;
				break;
			case 'h':
				bHelp = true;
				break;
			case 'e':
				bExact = true;
				countExact++;
				bError = bError || (countExact > 1);
				if (countExact > 1) printf("Error: either -e or -f but not both!\n");
				break;
			case 'f':
				bExact = false;
				countExact++;
				bError = bError || (countExact > 1);
				if (countExact > 1) printf("Error: either -e or -f but not both!\n");
				break;
			case 's':
				bSqlite = true;
				process_argwithopt(optarg, c, bError, sqfn, true);
				break;
			case 'p':
				bParam = true;
				param = optarg;
				break;
			case 'b':
				fpath = optarg;
				break;
			case 'g':
				bGraph = true;
				break;
			case 't':
				bTerm = true;
				term = optarg;
				break;
			case 'l':
				limitlen = atoi(optarg);
				break;
			case 'u':
				bFull = true;
				break;
			case 'd':
				bDebug = true;
				break;
			case '?':
				bError = true;
				break;
			default:
				break;
		}
    }
	if (bVersion)
	{
		printlicense();
		return 0;
	}
	if (bHelp || bError)
	{
		printhelp(extract_filename(argv[0]));
		return (bError ? 1 : 0);
	}
	if (!bSqlite)
	{
		printf("Error: -s is required.\n");
		bError = true;
	}
	if (!bTerm)
	{
		printf("Error: -t is required.\n");
		bError = true;
	}
	if (bError)
	{
		printhelp(extract_filename(argv[0]));
		return 1;
	}
	if (bSqlite && bTerm)
	{
		bError = process_query(sqfn, term, param, bExact, bFull, bDebug, limitlen, bGraph, fpath) > 0;
	}
	if (bError)
	{
		printhelp(extract_filename(argv[0]));
	}
	return bError;
}

