
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

// Advanced version of sqlquery to be GPL'ed

#include "std2qt.h"
#include "sqlqueryadv.h"
#include <QString>

sqlqueryadv::sqlqueryadv()
{
}

sqlqueryadv::~sqlqueryadv()
{
}

// Generate function call graph
// returns true if successful, false if failed
bool sqlqueryadv::search_funcgraph(QString searchstr, bool exactmatch, QString& xmlout, QString& dotout)
{

	sqlqueryresultlist result1, result2;
	QString xmltext = "<graph>";
	QString dottext = "digraph graphname {\n";
	int nodenum = 1;

	result1 = search(qt2str(searchstr), sqlresultCALLINGFUNC, exactmatch);
	result2 = search(qt2str(searchstr), sqlresultCALLEDFUNC, exactmatch);
	if (result1.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		return false;
	}
	else if (result2.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		return false;
	}

	xmltext += QString("<node fill=\"#e2ffff\" id=\"%1\" label=\"%2\"/>").arg(nodenum).arg(searchstr);
	dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#e2ffff\" shape=\"box\" ];\n").arg(nodenum).arg(searchstr);

	nodenum++;

	for (int i=0; i < result1.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"1\" source=\"%1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node1;\n").arg(nodenum);
		nodenum++;
	}
	for (int i=0; i < result2.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		dottext += QString("node1 -> node%1;\n").arg(nodenum);
		nodenum++;
	}
	xmltext += "</graph>";
	dottext += "}\n";
	xmlout = xmltext;
	dotout = dottext;
	return true;
}



