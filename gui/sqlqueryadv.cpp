
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


// Advanced version of sqlquery to be MPL'ed

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
bool sqlqueryadv::search_funcgraph(QString searchstr, bool exactmatch, QStringList& xmlout, QStringList& dotout, int levels)
{
	unsigned int i, j;
	sqlqueryresultlist result1, result2, result;
	QString xmltext = "<graph>";
	QString dottext = "digraph graphname {\n";
	unsigned int nodenum = 1, subrootnum;

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

	for (i=0; i < result1.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"1\" source=\"%1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result1.resultlist[i].symname.c_str()));
		dottext += QString("node%1 -> node1;\n").arg(nodenum);
		subrootnum = nodenum;
		nodenum++;
		if (levels == 2)
		{
			result = search(qt2str(result1.resultlist[i].symname.c_str()), sqlresultCALLINGFUNC, exactmatch);
			for (j=0; j < result.resultlist.size(); j++)
			{
				xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
					arg(nodenum).arg(QString(result.resultlist[j].symname.c_str()));
				xmltext += QString("<edge target=\"%1\" source=\"%2\"/>").arg(subrootnum).arg(nodenum);
				dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
					arg(nodenum).arg(QString(result.resultlist[j].symname.c_str()));
				dottext += QString("node%1 -> node%2;\n").arg(nodenum).arg(subrootnum);
				nodenum++;
			}
		}
	}
	for (i=0; i < result2.resultlist.size(); i++)
	{
		xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
			arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		xmltext += QString("<edge target=\"%1\" source=\"1\"/>").arg(nodenum);
		dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
			arg(nodenum).arg(QString(result2.resultlist[i].symname.c_str()));
		dottext += QString("node1 -> node%1;\n").arg(nodenum);
		subrootnum = nodenum;
		nodenum++;
		if (levels == 2)
		{
			result = search(qt2str(result2.resultlist[i].symname.c_str()), sqlresultCALLEDFUNC, exactmatch);
			for (j=0; j < result.resultlist.size(); j++)
			{
				xmltext += QString("<node fill=\"#ffffff\" id=\"%1\" label=\"%2\"/>").
					arg(nodenum).arg(QString(result.resultlist[j].symname.c_str()));
				xmltext += QString("<edge target=\"%1\" source=\"%2\"/>").arg(nodenum).arg(subrootnum);
				dottext += QString("node%1 [label=\"%2\" style=filled fillcolor=\"#ffffff\" shape=\"box\" ];\n").
					arg(nodenum).arg(QString(result.resultlist[j].symname.c_str()));
				dottext += QString("node%1 -> node%2;\n").arg(subrootnum).arg(nodenum);
				nodenum++;
			}
		}
	}
	xmltext += "</graph>";
	dottext += "}\n";
	xmlout.append(xmltext);
	dotout.append(dottext);
	return true;
}

bool sqlqueryadv::search_classinheritgraph(QString searchstr, bool exactmatch, QStringList& xmlout, QStringList& dotout)
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
	xmlout.append(xmltext);
	dotout.append(dottext);
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


