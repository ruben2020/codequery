
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <string>
#include "getopt2.h"
#include "csdbparser.h"
#include "cs2sq.h"
#include "ctagread.h"
#include "small_lib.h"
#include "swver.h"

#if 0 // test code for csdbparser - not needed
int test_csdbparser (void)
{
csdbparser csdbp;
std::string s;
long int num = 0;
csdbparser::enResult res;

//csdbp.setDebug(true);

int k = csdbparser::file_sanity_check("./cscope.out");
printf("%d\n",k);

csdbp.open_file("./cscope.out");
//csdbp.setup_srcfil_read();
do
	{
		csdbp.get_next_srcfil(&s);
		if (s.length() == 0) break;
		num++;
		//printf("\"%s\"\n",s.data());
	}
while (s.length() > 0);

printf("Total num of files = %ld\nBase path = %s\n\n", num, csdbp.getBasePath());

num = 0;
res = csdbp.setup_symbol_read();
if (res != csdbparser::resOK) printf("Error in setup_symbol_read: %d\n", res);
symdata_pack sp;
sp.valid = true;
while(sp.valid)
	{
		res = csdbp.get_next_symbol(&sp);
		if (res != csdbparser::resOK)
		{
			printf("Error in get_next_symbol: %d\n", res);
			break;
		}
		if (sp.valid)
		{
		printf("[[ %s, linenum=%ld, symbols=%d, \"%s\" ]]\n", sp.filename.c_str(), sp.line_num, sp.symbols.size(), sp.line_text.c_str());
		if (sp.symbols.size() > 0)
			{
			for (int i=0; i < sp.symbols.size(); i++)
				{
					printf("{%s, %s",
					sp.symbols[i].symbname.c_str(), sp.symbols[i].getTypeDesc());
					if (sp.symbols[i].sym_type == sym_data::symFuncCall)
						printf(", func[%s], macro[%s]",
					sp.symbols[i].calling_func.c_str(), sp.symbols[i].calling_macro.c_str());
					printf("} ");
				}
			printf("\n");
			}
		num += sp.symbols.size();
		}
	}
printf("Total number of symbols found = %ld\n",num);
}
#endif

int process_cscope(const char* cscopefn, const char* sqfn, bool debug)
{
	cs2sq::enResult res;
	cs2sq dbmaker;
	dbmaker.setDebug(debug);
	
	int k = csdbparser::file_sanity_check(cscopefn);
	printf("cscope.out sanity check %s\n",(k==csdbparser::resOK ? "OK" : "Error"));
	if (k == csdbparser::resUNSUPPORTED_PARAM) printf("Unsupported cscope parameters: -c is required. -q, -b and -R are optional. The rest should not be used.\n");
	if (k != csdbparser::resOK) return 1;

	dbmaker.open_csdb(cscopefn);
	
	k = dbmaker.test_csdb();
	printf("cscope.out detailed check %s\n",(k==0 ? "OK" : "Error"));
	if (k != 0)
	{
		printf("Please note that assembly code embedded in C/C++ files is unsupported.\n"
			"This might be the cause for this error.\n"
			"Please use the -d (for debug) argument to get more info.\n");
		return 1;
	}

	remove(sqfn);	
	res = dbmaker.open_db(sqfn);
	if (res != cs2sq::resOK) {printf("Error1! %d\n",res); return 1;}

	res = dbmaker.setup_tables();
	if (res != cs2sq::resOK) {printf("Error2! %d\n",res); return 1;}

	printf("Adding symbols ...\n");
	res = dbmaker.add_symbols();
	if (res != cs2sq::resOK) {printf("Error3! %d\n",res); return 1;}

	printf("Finalizing ...\n");
	res = dbmaker.finalize();
	if (res != cs2sq::resOK) {printf("Error4! %d\n",res); return 1;}
	return 0;
}

int process_ctags(const char* ctagsfn, const char* sqfn, bool debug)
{
	ctagread tagreader;
	ctagread::enResult restag;
	tagreader.setDebug(debug);
	restag = tagreader.open_files(sqfn, ctagsfn);
	if (restag != ctagread::resOK) {printf("Error!!!\n"); return 1;}
	printf("Processing ctags file ...\n");
	restag = tagreader.process_ctags();
	if (restag != ctagread::resOK) {printf("Error!!!\n"); return 1;}
	printf("Finalizing ...\n");
	restag = tagreader.finalize();
	if (restag != ctagread::resOK) {printf("Error!!!\n"); return 1;}
	return 0;
}

void printhelp(const char* str)
{
	printf("Usage: %s [-s <sqdbfile> [-c <cscope.out>] [-t <ctags>]] [-p] [-d] [-v] [-h]\n\n", str);
	printf("options:\n");
	printf("  -s : CodeQuery sqlite3 db file path\n");
	printf("  -c : cscope.out file path\n");
	printf("  -t : ctags tags file path\n");
	printf("  -p : \"vacuum\", compact database (may take more time)\n");
	printf("  -d : debug\n");
	printf("  -v : version\n");
	printf("  -h : help\n\n");
	printf("The combinations possible are -s -c, -s -t, -s -c -t\n");
	printf("The additional optional arguments are -p and -d\n");
	printf("if -c is present then sqdbfile need not exist. It will be created.\n");
	printf("if -t is present but not -c, then sqdbfile has to exist. Ctags info will be added to it.\n\n");
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

void process_argwithopt(char* thisOpt, int option, bool& err, std::string& fnstr, bool filemustexist)
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

int main(int argc, char *argv[])
{
    int c;
    bool bHelp, bSqlite, bCscope, bCtags, bDebug, bError, bVacuum, bVersion;
	bHelp = (argc <= 1);
	bVersion = false;
	bSqlite = false;
	bCscope = false;
	bCtags = false;
	bDebug = false;
	bError = false;
	bVacuum = false;
	std::string sqfn, csfn, ctfn;

    while ((c = getopt2(argc, argv, "s:c:t:pdvh")) != -1)
    {
		switch(c)
		{
			case 'v':
				bVersion = true;
				break;
			case 'h':
				bHelp = true;
				break;
			case 's':
				bSqlite = true;
				process_argwithopt(optarg, c, bError, sqfn, false);
				break;
			case 'c':
				bCscope = true;
				process_argwithopt(optarg, c, bError, csfn, true);
				break;
			case 't':
				bCtags = true;
				process_argwithopt(optarg, c, bError, ctfn, true);
				break;
			case 'd':
				bDebug = true;
				break;
			case 'p':
				bVacuum = true;
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
	if ((bCscope == false)&&(bCtags == false))
	{
		printf("Error: Either -c or -t or both must be present. Both are missing.\n");
		bError = true;
	}	
	if ((!bCscope) && (fileexists(sqfn.c_str()) == false))
	{
		printf("Error: File %s doesn't exist.\n", sqfn.c_str());
		printf("Error: Without -c, sqdbfile must exist.\n");
		bError = true;
	}
	if (bError)
	{
		printhelp(extract_filename(argv[0]));
		return 1;
	}
	if (bCscope)
	{
		if (process_cscope(csfn.c_str(), sqfn.c_str(), bDebug) == 1)
			return 1;
	}
	if (bCtags)
	{
		if (process_ctags(ctfn.c_str(), sqfn.c_str(), bDebug) == 1)
			return 1;
	}
	if (bVacuum)
	{
		printf("Compacting database ...\n");
		if (sqlbase::vacuum(sqfn.c_str(), bDebug) != 0)
			return 1;
	}
	else
	{
		if (sqlbase::analyze(sqfn.c_str(), bDebug) != 0)
			return 1;
	}
	return 0;
}

