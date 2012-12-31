/*
 *	Maze
 *	Copyright 2009, 2010, 2011 Marco Mandrioli
 *
 *	This file is part of Maze.
 *
 *	Maze is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Maze is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Maze.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _XML_H
#define _XML_H

#include <vector>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>

#include "header.h"

using namespace std;

class XML {
private:
	static xmlDocPtr document;
	static xmlNodePtr current;

	static int ParseMaze(const char *, xmlDocPtr *);
	static bool ValidateXML(const char *);
	static xmlXPathObjectPtr GetNodeSet(xmlDocPtr, xmlChar *);
public:
	static int GetMazeName(const char *, char **);
	static int GetMazeData(const char *, vector<string> *);
};

#endif /* _XML_H */