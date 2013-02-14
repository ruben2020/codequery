
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


#include <string>
#include "optlist.h"
#include "csdbparser.h"
#include "cs2sq.h"
#include "ctagread.h"
#include "small_lib.h"

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
	printf("cscope.out sanity check %s\n",(k==0 ? "OK" : "Error"));
	if (k != 0) return 1;

	dbmaker.open_csdb(cscopefn);
	
	k = dbmaker.test_csdb();
	printf("cscope.out detailed check %s\n",(k==0 ? "OK" : "Error"));
	if (k != 0) return 1;

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
	printf("Usage: %s [-s <sqdbfile> [-c <cscope.out>] [-t <ctags>]] [-v] [-d] [-h]\n\n", str);
	printf("options:\n");
	printf("  -s : sqlite3 db file path\n");
	printf("  -c : cscope.out file path\n");
	printf("  -t : ctags tags file path\n");
	printf("  -v : \"vacuum\", compact database (may take more time)\n");
	printf("  -d : debug\n");
	printf("  -h : help\n\n");
	printf("The combinations possible are -s -c, -s -t, -s -c -t\n");
	printf("The additional optional arguments are -v and -d\n");
	printf("if -c is present then sqdbfile need not exist. It will be created.\n");
	printf("if -t is present but not -c, then sqdbfile has to exist. Ctags info will be added to it.\n\n");
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

void process_argwithopt(option_t* thisOpt, bool& err, std::string& fnstr, bool filemustexist)
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

int main(int argc, char *argv[])
{
    option_t *optList=NULL, *thisOpt=NULL;
    bool bHelp, bSqlite, bCscope, bCtags, bDebug, bError, bVacuum;
	bHelp = (argc <= 1);
	bSqlite = false;
	bCscope = false;
	bCtags = false;
	bDebug = false;
	bError = false;
	bVacuum = false;
	std::string sqfn, csfn, ctfn;

    /* get list of command line options and their arguments */
    optList = GetOptList(argc, argv, (char*)"s:c:t:vdh");

    /* display results of parsing */
    while (optList != NULL)
    {
        thisOpt = optList;
        optList = optList->next;
		
		switch(thisOpt->option)
		{
			case 'h':
				bHelp = true;
				break;
			case 's':
				bSqlite = true;
				process_argwithopt(thisOpt, bError, sqfn, false);
				break;
			case 'c':
				bCscope = true;
				process_argwithopt(thisOpt, bError, csfn, true);
				break;
			case 't':
				bCtags = true;
				process_argwithopt(thisOpt, bError, ctfn, true);
				break;
			case 'd':
				bDebug = true;
				break;
			case 'v':
				bVacuum = true;
				break;
			default:
				break;
		}
        free(thisOpt);    /* done with this item, free it */
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
	return 0;
}

