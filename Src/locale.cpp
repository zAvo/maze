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

using namespace std;


int Locale::GetFileList(const char *fld, vector<string> *list, vector<string> *mnames) {
	int counter = 0;
	char *tmpname = NULL;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	string folder(fld);
	folder.append("/*.maze");

	if ((hFind = FindFirstFile(folder.c_str(), &findFileData)) == INVALID_HANDLE_VALUE) {
		printf("\nError: Mazes not found (%d)", GetLastError());
		
		return -1;
	}

	do {
		string fname(fld);
		fname.append(findFileData.cFileName);

		if (XML::GetMazeName(fname.c_str(), &tmpname) == 0) {
			mnames->push_back(tmpname);
			list->push_back(findFileData.cFileName);

			counter++;
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	if (GetLastError() != ERROR_NO_MORE_FILES) {
		printf("\nError while searching for Mazes (%d)", GetLastError());
		
		return -2;
	}

	FindClose(hFind);

	return counter;
}
