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


#ifndef _ENUMS_H
#define _ENUMS_H

enum Status {
	// Wiimote
	WiiNo			= 0,
	WiiMoteOK		= 1,
	WiiBoardNo		= 2,
	WiiBoardOK		= 3,
	WiiOK			= 4,
	WiiERR			= 5,
	
	// Game
	GameLoad		= 10,
	GameOK			= 11,
	GameERR			= 12,

	// Level
	LevelComplete	= 20,
	LevelLost		= 21,
	LevelTimeUp		= 22,
	LevelNoLevels	= 23,

	// Menu
	MenuOK			= 30,
	MenuIn			= 31,
	MenuERR			= 32,

	// General
	Quit			= 40,
	ERR				= 41
};

#endif /* _ENUMS_H */