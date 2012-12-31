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


#ifndef _WII_H
#define _WII_H

#include <wiiuse.h>

#include "header.h"

using namespace std;

class Wii {
private:
	static wiimote *wiimote_ptr;
	static struct wii_board_t* wii_board;
	static bool balanceBoard;

	Wii();
	~Wii();

	static void BoardInit();
	static void HandleEvent();
	static void HandleLeds();
	
public:
	static enum Status Init();
	static enum Status Poll(enum Status);

	static float GetTiltX();
	static float GetTiltY();
	static float GetWeight();
};

#endif /* _WII_H */