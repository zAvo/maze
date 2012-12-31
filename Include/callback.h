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


#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "header.h"

using namespace std;

class Callback {
private:	
	static int frame;
	static int time;
	static int timebase;
	static short game_status;

public:
//	Callback ();
//	~Callback ();

	static void RenderScene ();
	static void Idle ();
	static void ProcessNormalKeys ( unsigned char, int, int );
	static void PressKey ( int, int, int );
	static void SetGameStatus ( short status );
	static short GetGameStatus ();
};

#endif /* _CALLBACK_H */