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


#ifndef _MENU_H
#define _MENU_H

#include <vector>

#include "header.h"


class MenuMain {
private:
	static vector<string> name;
	static int active;

	static int Init();
public:
	static void Prev();
	static void Next();
	static int Do();

	friend class Menu;
};

class MenuLevels {
private:
	static vector<string> fname, mname;
	static int active;

	static int Init();
public:
	static void Prev();
	static void Next();
	static const char *GetActiveLevel();
	static const char *GetNextLevel();

	friend class Menu;
};

class MenuGame {
private:
	static vector<string> name;
	static int active;

	static int Init();
public:
	static void Prev();
	static void Next();
	static int Do();

	friend class Menu;
};

class Menu {
private:
	static enum MenuType {
		Main,
		Levels,
		Game
	} active;

public:
	static enum Status Init();
	static void SetMenuMain();
	
	static void Prev();
	static void Next();
	static enum Status Do();
	static enum Status Escape();
	static void Draw();
};

#endif /* _MENU_H */
