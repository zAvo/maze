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


#include "../Include/header.h"

/* data:
maze:			name, time, size-x, size-y
graphicSet:		name
start:			x, y
goal:			x, y, radius
walls:			#, x1, y1, x2, y2
holes:			#, number, x, y, radius
*/

bool XML::ValidateXML(const char *docname) {
	xmlDocPtr doc;
	xmlSchemaParserCtxtPtr ctxt;
	xmlSchemaPtr schema = NULL;
	int ret = -1;

	ctxt = xmlSchemaNewParserCtxt(MAZE_XML_SCHEMA);
	schema = xmlSchemaParse(ctxt);

	xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadFile(docname, NULL, 0);

	if (doc == NULL) {
		fprintf(stderr, "Could not parse %s\n", docname);
	}
	else {
		xmlSchemaValidCtxtPtr ctxt;

		ctxt = xmlSchemaNewValidCtxt(schema);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		if (ret == 0) {
			printf("%s validates\n", docname);
		} else if (ret > 0) {
			printf("%s fails to validate\n", docname);
		} else {
			printf("%s validation generated an internal error\n", docname);
		}
		
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}

	if (ret == 0)
		return true;
	else return false;
}

int XML::GetMazeData(const char *docname, vector<string> *ret) {
	xmlDocPtr doc = NULL;
	xmlNodePtr cur;
	xmlXPathObjectPtr nodes;
	string tmp_str;
	char tmp2[MAX_STR_BUFF];

	if (!ValidateXML(docname))
		return -1;

	if (ParseMaze(docname, &doc) != 0)
		return -1;

	cur = xmlDocGetRootElement(doc);

	/* maze */
	// name
	xmlChar *tmp = xmlGetProp(cur, (const xmlChar *)"name");
	tmp_str.assign((const char *)tmp);
	ret->push_back(tmp_str);
	xmlFree(tmp);

	// time
	tmp = xmlGetProp(cur, (const xmlChar *)"time");
	tmp_str.assign((const char *)tmp);
	ret->push_back(tmp_str);
	xmlFree(tmp);

	// size-x
	tmp = xmlGetProp(cur, (const xmlChar *)"size-x");
	tmp_str.assign((const char *)tmp);
	ret->push_back(tmp_str);
	xmlFree(tmp);

	// size-y
	tmp = xmlGetProp(cur, (const xmlChar *)"size-y");
	tmp_str.assign((const char *)tmp);
	ret->push_back(tmp_str);
	xmlFree(tmp);

	int i;
	/* graphicSet */
	nodes = GetNodeSet(doc, (xmlChar *)"//maze:graphicSet");
	for (i=0; i < nodes->nodesetval->nodeNr; i++) {
		// name
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"name");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
	}
	xmlXPathFreeObject(nodes);

	/* start */
	nodes = GetNodeSet(doc, (xmlChar *)"//maze:start");
	for (i=0; i < nodes->nodesetval->nodeNr; i++) {
		// x
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"x");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// y
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"y");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
	}
	xmlXPathFreeObject(nodes);

	/* goal */
	nodes = GetNodeSet(doc, (xmlChar *)"//maze:goal");
	for (i=0; i < nodes->nodesetval->nodeNr; i++) {
		// x
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"x");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// y
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"y");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);

		// radius
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"radius");
		if (tmp == NULL) {
			sprintf_s((char *)tmp2, MAX_STR_BUFF, "%d", MAZE_HOLE_RADIUS);
			tmp_str.assign((const char *)tmp2);
		}
		else tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
	}
	xmlXPathFreeObject(nodes);

	/* walls */
	nodes = GetNodeSet(doc, (xmlChar *)"//maze:w");
	// # of walls
	sprintf_s((char *)tmp2, MAX_STR_BUFF, "%d", nodes->nodesetval->nodeNr);
	tmp_str.assign((const char *)tmp2);
	ret->push_back(tmp_str);

	for (i=0; i < nodes->nodesetval->nodeNr; i++) {
		// x1
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"x1");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// y1
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"y1");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);

		// x2
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"x2");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// y2
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"y2");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
	}
	xmlXPathFreeObject(nodes);

	/* holes */
	nodes = GetNodeSet(doc, (xmlChar *)"//maze:h");
	// # of holes
	sprintf_s((char *)tmp2, MAX_STR_BUFF, "%d", nodes->nodesetval->nodeNr);
	tmp_str.assign((const char *)tmp2);
	ret->push_back(tmp_str);

	for (i=0; i < nodes->nodesetval->nodeNr; i++) {
		// number
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"number");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);

		// x
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"x");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// y
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"y");
		tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
		
		// radius
		tmp = xmlGetProp(nodes->nodesetval->nodeTab[i], (const xmlChar *)"radius");
		if (tmp == NULL) {
			sprintf_s((char *)tmp2, MAX_STR_BUFF, "%d", MAZE_HOLE_RADIUS);
			tmp_str.assign((const char *)tmp2);
		}
		else tmp_str.assign((const char *)tmp);
		ret->push_back(tmp_str);
		xmlFree(tmp);
	}
	xmlXPathFreeObject(nodes);

	return 0;
}

int XML::GetMazeName(const char *docname, char **res) {
	xmlDocPtr doc = NULL;
	xmlNodePtr root;

	if (!XML::ValidateXML(docname))
		return -1;
	
	if (XML::ParseMaze(docname, &doc) != 0)
		return -2;

	root = xmlDocGetRootElement(doc);
	xmlChar *uri = xmlGetProp(root, (const xmlChar *)"name");
	(*res) = (char *)uri;

	xmlFreeDoc(doc);

	return 0;
}

int XML::ParseMaze(const char *docname, xmlDocPtr *doc) {
	xmlNodePtr cur;
//	xmlXPathObjectPtr nodes;

	(*doc) = xmlParseFile(docname);
	if ((*doc) == NULL) {
		fprintf(stderr,"Document not parsed successfully. \n");
		
		return -1;
	}
	cur = xmlDocGetRootElement((*doc));
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");

		return 1;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *)"maze")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		
		return 2;
	}
	return 0;
}

xmlXPathObjectPtr XML::GetNodeSet(xmlDocPtr doc, xmlChar *xpath) {
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	xmlXPathRegisterNs(context, (xmlChar *)"maze", (xmlChar *)"http://lzavol.altervista.org/schema/maze"); 
	result = xmlXPathEvalExpression(xpath, context);
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}

	return result;
}
