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


#ifndef _GAME_H
#define _GAME_H

#include "header.h"


class Game : IGame<Game> {
private:
	static enum Status status;
	static int t_start;
	static int t_total;
	static bool pause;

	static enum Status StartLevel(const char *);
	static void ContinueGame();
	static void PauseGame();
public:
	static enum Status Init(int *, char **);
	static void DrawScene();
	static void Idle();
	static int TimeLeft();
	static void ProcessNormalKeys(unsigned char, int, int);
	static void ProcessSpecialKeys(int, int, int);
};

#endif /* _GAME_H */
