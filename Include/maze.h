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


#ifndef _MAZE_H
#define _MAZE_H

#include <vector>

#include "header.h"

using namespace std;


class CMazeData {
private:
	static string name;
	static CCoord<unsigned short> size;
	static CCoord<double> start;
	static int time;
	static CMazeBall ball;
	static CMazeHole goal;
	static vector<CMazeWall> walls;
	static vector<CMazeHole> holes;

public:
	static void Init(char *);
	static string Name();
	static CCoord<unsigned short> Size();
	static CCoord<double> Start();
	static CMazeHole Goal();
	static int Time();
	static double BallSpeed();
	static double BallRadius();
	const vector<CMazeWall>& Walls() const { return walls; }
	const vector<CMazeHole>& Holes() const { return holes; }

	friend class Maze;
};

class Maze {
private:
	static CMazeData data;

public:
	Maze(char *filename);
	~Maze();

	static int Time();
	static int Load(const char *);
	static void Draw();
	static void Free();

	static CMazeData Data();
};

#endif /* _MAZE_H */