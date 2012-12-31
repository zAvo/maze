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

enum Menu::MenuType Menu::active;

void Menu::SetMenuMain() { active = Main; }

enum Status Menu::Init() {
	active = Main;
	int ret;

	if (MenuMain::Init() != 0)
		return MenuERR;

	ret = MenuLevels::Init();
	if (ret == -2)
		return MenuERR;
	else if (ret == -1)
		return LevelNoLevels;
	if (MenuGame::Init() != 0)
		return MenuERR;

	return MenuOK;
}

void Menu::Prev() {
	switch (active) {
		case Main:
			MenuMain::Prev();
			break;
		case Levels:
			MenuLevels::Prev();
			break;
		case Game:
			MenuGame::Prev();
			break;
	}
}

void Menu::Next() {
	switch (active) {
		case Main:
			MenuMain::Next();
			break;
		case Levels:
			MenuLevels::Next();
			break;
		case Game:
			MenuGame::Next();
			break;
	}
}

enum Status Menu::Do() {
	int val;
	switch (active) {
		case Main:
			val = MenuMain::Do();
			switch (val) {
				case 0:
					active = Game;
					return GameLoad;
				case 1:
					active = Levels;
					MenuLevels::active = 0;
					return MenuIn;
				case 2:
					return Quit;
				default:
					return MenuERR;
		}
		case Levels:
			if (MenuLevels::active == MenuLevels::mname.size() - 1) {
				active = Main;
				return MenuIn;
			} else {
				active = Game;
				return GameLoad;
			}
		case Game:
			val = MenuGame::Do();
			switch (val) {
				case 0:
					return GameOK;
				case 1:
					return GameLoad;
				case 2:
					active = Main;
					return MenuIn;
				default:
					return MenuERR;
		}
		default:
			return MenuERR;
	}
}

enum Status Menu::Escape() {
	switch (active) {
		case Main:
			return Quit;
		case Levels:
			active = Main;
			return MenuIn;
		case Game:
			return GameOK;
		default:
			return MenuERR;
	}
}

void Menu::Draw() {
	switch (active) {
		case Main:
			Graphics::DrawMenu(MenuMain::name, MenuMain::active, false, false);
			break;
		case Levels:
			Graphics::DrawMenu(MenuLevels::mname, MenuLevels::active, true, false);
			break;
		case Game:
			Graphics::DrawScene();
			Graphics::DrawMenu(MenuGame::name, MenuGame::active, false, true);
			break;
	}
}


int MenuMain::active;
vector<string> MenuMain::name;

int MenuMain::Init() {
	active = 0;

	name.push_back("Play");
	name.push_back("Select level");
	name.push_back("Quit");

	return 0;
}

void MenuMain::Prev() {
	active--;
	if (active < 0)
		active = (int)name.size() - 1;
}

void MenuMain::Next() {
	active++;
	if (active >= (int)name.size())
		active = 0;
}

int MenuMain::Do() {
	switch (active) {
		case 0:
			active = 0;
			return 0;
		case 1:
			active = 0;
			return 1;
		case 2:
			return 2;
		default:
			active = 0;
			return -1;
	}
}


int MenuLevels::active;
vector<string> MenuLevels::fname;
vector<string> MenuLevels::mname;

int MenuLevels::Init() {
	active = 0;
	int ret;

	ret = Locale::GetFileList(MAZE_FOLDER, &fname, &mname);
	if (ret == -2) {
		printf("\nError initializing Menus. Cannot search for maze files.");

		return -1;
	}
	else if (ret == -1) {
		printf("\nNo maze files found.");

		return -1;
	}
	else if (ret == 0) {
		printf("\nNo valid maze files found.");

		return -1;
	}

	mname.push_back("Go to main menu");

	return 0;
}

void MenuLevels::Prev() {
	active--;
	if (active < 0)
		active = (int)mname.size() - 1;
}

void MenuLevels::Next() {
	active++;
	if (active >= (int)mname.size())
		active = 0;
}

const char *MenuLevels::GetActiveLevel() { return fname[active].c_str(); }

const char *MenuLevels::GetNextLevel() {
	active++;
	if (active >= (int)fname.size())
		active = 0;

	return fname[active].c_str();
}


int MenuGame::active;
vector<string> MenuGame::name;

int MenuGame::Init() {
	active = 0;

	name.push_back("Continue");
	name.push_back("Restart level");
	name.push_back("Go to main menu");

	return 0;
}

void MenuGame::Prev() {
	active--;
	if (active < 0)
		active = (int)name.size() - 1;
}

void MenuGame::Next() {
	active++;
	if (active >= (int)name.size())
		active = 0;
}

int MenuGame::Do() {
	switch (active) {
		case 0:
			active = 0;
			return 0;
		case 1:
			active = 0;
			return 1;
		case 2:
			active = 0;
			return 2;
		default:
			active = 0;
			return -1;
	}
}