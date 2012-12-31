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


#ifndef _TYPES_H
#define _TYPES_H

#include "header.h"

using namespace std;


template <class T>
class CCoord {
private:
public:
	T x, y;
};

class CColor {
private:
public:
	CColor(double, double, double, double);
	double r, g, b, a;
};

class CMazeBall {
private:
	double speed, radius;

public:
	void Init ( double, double );

	double Speed () const;
	double Radius () const;
};

class CMazeWall {
private:
	CCoord<double> pos;
	CCoord<double> half_size;

public:
	void Init ( CCoord<double>, CCoord<double> );

	CCoord<double> Position () const;
	CCoord<double> HalfSize () const;
};

class CMazeHole {
private:
	CCoord<double> pos;
	double radius;
	int number;

public:
	void Init ( CCoord<double>, double );
	void Init ( CCoord<double>, double, int );

	CCoord<double> Position () const;
	double Radius () const;
	int Number() const;
};

#endif /* _TYPES_H */