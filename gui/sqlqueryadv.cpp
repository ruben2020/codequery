
/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
#include <QSet>
#include <QList>

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

	unique_symnames(result1);
	unique_symnames(result2);

	xmltext += QString("<node fill=\"#e2ffff\" id=\"%1\" label=\"%2\"/>").arg(nodenum).arg(searchstr);
	dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#e2ffff\" shape=\"box\" ];\n").arg(nodenum).arg(searchstr);

	nodenum++;

	for (unsigned int i=0; i < result1.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"1\" source=\"%1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node1;\n").arg(nodenum);
		nodenum++;
	}
	for (unsigned int i=0; i < result2.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		dottext += QString("node1 -> node%1;\n").arg(nodenum);
		nodenum++;
	}
	xmltext += "</graph>";
	dottext += "}\n";
	xmlout = xmltext;
	dotout = dottext;
	return true;
}

bool sqlqueryadv::search_classinheritgraph(QString searchstr, bool exactmatch, QString& xmlout, QString& dotout)
{

	sqlqueryresultlist result_children, result_parent1, result_cousins1, result_parent2;
	QString xmltext = "<graph>";
	QString dottext = "digraph graphname {\n";
	int nodenum = 1;
	int parent1 = 0;

	xmltext += QString("<node fill=\"#e2ffff\" id=\"%1\" label=\"%2\"/>").arg(nodenum).arg(searchstr);
	dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#e2ffff\" shape=\"box\" ];\n").arg(nodenum).arg(searchstr);
	nodenum++;

	result_children = search(qt2str(searchstr), sqlresultCHILDCLASS, exactmatch);
	if (result_children.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		return false;
	}
	result_parent1 = search(qt2str(searchstr), sqlresultPARENTCLASS, exactmatch);
	if (result_parent1.result_type == sqlqueryresultlist::sqlresultERROR)
	{
		return false;
	}
	if (result_parent1.resultlist.size() > 0)
	{
		result_parent2 = search(result_parent1.resultlist[0].symname, sqlresultPARENTCLASS, exactmatch);
		if (result_parent2.result_type == sqlqueryresultlist::sqlresultERROR)
		{
			return false;
		}
		result_cousins1 = search(result_parent1.resultlist[0].symname, sqlresultCHILDCLASS, exactmatch);
		if (result_cousins1.result_type == sqlqueryresultlist::sqlresultERROR)
		{
			return false;
		}
	}
	unique_symnames(result_children);
	unique_symnames(result_parent1);
	unique_symnames(result_parent2);
	unique_symnames(result_cousins1);
	remove_symname(result_cousins1, qt2str(searchstr)); // I am not my own cousin
	for (unsigned int i=0; i < result_children.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result_children.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"1\" source=\"%1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result_children.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node1 [arrowhead=\"empty\"];\n").arg(nodenum);
		nodenum++;
	}
	for (unsigned int i=0; i < result_parent1.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result_parent1.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result_parent1.resultlist[i].symname.c_str()));
		dottext += QString("node1 -> node%1 [arrowhead=\"empty\"];\n").arg(nodenum);
		if (i == 0) parent1 = nodenum;
		nodenum++;
	}
	for (unsigned int i=0; i < result_parent2.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result_parent2.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"%2\"/>").arg(nodenum).arg(parent1);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result_parent2.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node%2 [arrowhead=\"empty\"];\n").arg(parent1).arg(nodenum);
		nodenum++;
	}
	for (unsigned int i=0; i < result_cousins1.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result_cousins1.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"%2\"/>").arg(parent1).arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result_cousins1.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node%2 [arrowhead=\"empty\"];\n").arg(nodenum).arg(parent1);
		nodenum++;
	}
	xmltext += "</graph>";
	dottext += "}\n";
	xmlout = xmltext;
	dotout = dottext;
	return true;
}

// make the list of symnames unique, no elements repeated
void sqlqueryadv::unique_symnames(sqlqueryresultlist& res)
{
	QSet<QString> setstr;
	sqlqueryresultlist out;
	sqlqueryresult item;
	for(unsigned int i=0; i < res.resultlist.size(); i++)
	{
		setstr.insert(str2qt(res.resultlist[i].symname));
	}
	QList<QString> lststr = setstr.toList();
	for(int i=0; i < lststr.size(); i++)
	{
		item.symname = qt2str(lststr[i]);
		out.resultlist.push_back(item);
	}
	res = out;
}

// remove a symname from the list
void sqlqueryadv::remove_symname(sqlqueryresultlist& res, tStr name)
{
	std::vector<sqlqueryresult>::iterator it;
	for (it = res.resultlist.begin(); it != res.resultlist.end(); it++)
	{
		if (it->symname.compare(name) == 0)
		{
			res.resultlist.erase(it);
			break;
		}
	}
}


