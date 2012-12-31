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


void CMazeBall::Init ( double _radius, double _speed ) {
	radius = _radius;
	speed = _speed;
}

double CMazeBall::Speed () const { return speed; }
double CMazeBall::Radius () const { return radius; }


void CMazeWall::Init ( CCoord<double> _pos, CCoord<double> _half_size ) {
	pos = _pos;
	half_size = _half_size;
}

CCoord<double> CMazeWall::Position () const { return pos; }
CCoord<double> CMazeWall::HalfSize () const { return half_size; }


void CMazeHole::Init ( CCoord<double> _pos, double _radius ) {
	pos = _pos;
	radius = _radius;
}

void CMazeHole::Init ( CCoord<double> _pos, double _radius, int _number ) {
	pos = _pos;
	radius = _radius;
	number = _number;
}

CCoord<double> CMazeHole::Position () const { return pos; }
double CMazeHole::Radius () const { return radius; }
int CMazeHole::Number() const { return number; }

CColor::CColor(double red, double green, double blue, double alpha) {
	r = red;
	g = green;
	b = blue;
	a = alpha;
}
