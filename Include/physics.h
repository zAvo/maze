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


#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "header.h"

using namespace std;

class Physics {
private:
	static CCoord<double> pos;
	static CCoord<double> accel;
	static CCoord<double> speed;
	static int time;
	static int time_old;
	static int elapsed;
	static double ballradius;

	static void ComputeAccel();
	static void ComputeSpeed();
	static void CheckCollision(CCoord<double>);
	static enum Status CheckHoles();

public:
	Physics();
	~Physics();

	static void Init ();
	static void ContinueGame();
	static enum Status ComputePosition();
	static CCoord<double> GetPos();
	static void SetDotSpeed(double);
};

#endif /* _PHYSICS_H */