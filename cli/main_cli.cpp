
/*
 * CodeQuery
 * Copyright (C) 2013 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "small_lib.h"
#include "optlist.h"
#include "sqlquery.h"
#include "swver.h"


void printhelp(const char* str)
{
	printf("Usage: %s [-s <sqdbfile> [-p <n>] [-t <term>] -[e|f] ] [-u] [-d] [-v] [-h]\n\n", str);
	printf("options:\n");
	printf("  -s : CodeQuery sqlite3 db file path\n");
	printf("  -p : parameter is a number denoted by n\n");
	printf("       default n = 1 (Symbol)\n");
	printf("  -t : search term without spaces\n");
	printf("       if Exact Match is switched off, wild card\n");
	printf("       searches are possible. Use * and ?\n");
	printf("  -e : Exact Match switched ON \n");
	printf("       Case-sensitive\n");
	printf("  -f : Exact Match switched OFF (fuzzy search)\n");
	printf("       Case-insensitive with wild card search (default)\n");
	printf("  -u : show full file path instead of file name\n");
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

void process_argwithopt(option_t* thisOpt, bool& err, tStr& fnstr, bool filemustexist)
{
	if (thisOpt->argument != NULL)
	{
		fnstr = thisOpt->argument;
		if (filemustexist && (fileexists(fnstr.c_str()) == false))
		{
			printf("Error: File %s doesn't exist.\n", fnstr.c_str());
			err = true;
		}
	}
	else
	{
		printf("Error: -%c used without file option.\n", thisOpt->option);
		err = true;
	}
}

int process_query(tStr sqfn, tStr term, tStr param, bool exact, bool full, bool debug)
{
	if ((sqfn.empty())||(term.empty())||(param.empty())) return 1;
	int retVal = 0;
	std::unique_ptr<sqlquery> sq(new sqlquery);
	sqlquery::en_filereadstatus filestatus = sq->open_dbfile(sqfn);
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
	resultlst = sq->search(term, (sqlquery::en_queryType) intParam, exact);
	if (resultlst.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		printf("Error: SQL Error! %s!\n", resultlst.sqlerrmsg.c_str());
		return 1;	
	}
	for(std::vector<sqlqueryresult>::iterator it = resultlst.resultlist.begin();
		it != resultlst.resultlist.end(); it++)
	{
		switch(resultlst.result_type)
		{
			case sqlqueryresultlist::sqlresultFULL:
				printf("%s\t%s:%s\t%s\n", 
						it->symname.c_str(), 
						(full ? it->filepath.c_str() : it->filename.c_str()),
						it->linenum.c_str(),
						it->linetext.c_str());
				break;	
			case sqlqueryresultlist::sqlresultFILE_LINE:
				printf("%s:%s\t%s\n",
						(full ? it->filepath.c_str() : it->filename.c_str()),
						it->linenum.c_str(),
						it->linetext.c_str());
				break;	
			case sqlqueryresultlist::sqlresultFILE_ONLY:
				printf("%s\n", it->filepath.c_str());
				break;	
		}
	}
	return retVal;
}

int main(int argc, char *argv[])
{
    option_t *optList=NULL, *thisOpt=NULL;
    bool bSqlite, bParam, bTerm, bExact, bFull, bDebug, bVersion, bHelp, bError;
    int countExact = 0;
	bSqlite = false;
	bParam = false;
	bTerm = false;
	bExact = false;
	bFull = false;
	bDebug = false;
	bVersion = false;
	bHelp = (argc <= 1);
	bError = false;
	tStr sqfn, param = "1", term;

    /* get list of command line options and their arguments */
    optList = GetOptList(argc, argv, (char*)"s:p:t:efudvh");

    /* display results of parsing */
    while (optList != NULL)
    {
        thisOpt = optList;
        optList = optList->next;
		
		switch(thisOpt->option)
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
				process_argwithopt(thisOpt, bError, sqfn, true);
				break;
			case 'p':
				bParam = true;
				param = thisOpt->argument;
				break;
			case 't':
				bTerm = true;
				term = thisOpt->argument;
				break;
			case 'u':
				bFull = true;
				break;
			case 'd':
				bDebug = true;
				break;
			default:
				break;
		}
        free(thisOpt);    /* done with this item, free it */
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
		bError = process_query(sqfn, term, param, bExact, bFull, bDebug) > 0;
	}
	if (bError)
	{
		printhelp(extract_filename(argv[0]));
	}
	return bError;
}

