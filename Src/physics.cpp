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

CCoord<double> Physics::pos;
CCoord<double> Physics::accel;
CCoord<double> Physics::speed;
int Physics::time;
int Physics::time_old;
int Physics::elapsed;

Physics::Physics() {}

void Physics::Init() {
	pos.x = Maze::Data().Start().x;
	if (pos.x < Maze::Data().BallRadius() - 1.0f)
		pos.x = Maze::Data().BallRadius() - 1.0f;
	else if (pos.x > 1.0f - Maze::Data().BallRadius())
		pos.x = 1.0f - Maze::Data().BallRadius();
	
	pos.y = Maze::Data().Start().y;
	if (pos.y > 1.0f - Maze::Data().BallRadius())
		pos.y = 1.0f - Maze::Data().BallRadius();
	else if (pos.y < Maze::Data().BallRadius() - 1.0f)
		pos.y = Maze::Data().BallRadius() - 1.0f;

	accel.x = 0.0f;
	accel.y = 0.0f;
	speed.x = 0.0f;
	speed.y = 0.0f;

	time_old = Graphics::Time();
	time = Graphics::Time();
}

Physics::~Physics() {
}

void Physics::ComputeAccel() {
	double x = VAL_TO_RAD(Wii::GetTiltX());
	double y = VAL_TO_RAD(Wii::GetTiltY());

	if (fabs(x) > REST_VALUE)
		accel.x = sin(x) * cos(x) * Maze::Data().BallSpeed();
	else
		accel.x = 0.0f;

	if (fabs(y) > REST_VALUE )
		accel.y = sin(y) * cos(y) * Maze::Data().BallSpeed();
	else
		accel.y = 0.0f;
}

void Physics::ContinueGame() {
	time = Graphics::Time();
}

void Physics::ComputeSpeed() {
	ComputeAccel();

	time_old = time;
	time = Graphics::Time();
	elapsed = time - time_old;

	speed.x += accel.x * elapsed;
	speed.y += accel.y * elapsed;
}

enum Status Physics::ComputePosition() {
	ComputeSpeed();

	CCoord<double> temp;
	temp.x = pos.x + speed.x * elapsed + 0.5 * accel.x * elapsed * elapsed;
	temp.y = pos.y + speed.y * elapsed + 0.5 * accel.y * elapsed * elapsed;
	CheckCollision(temp);
	return CheckHoles();
}

enum Status Physics::CheckHoles () {
	const vector<CMazeHole> holes = Maze::Data ().Holes ();
	for ( unsigned short i=0; i<holes.size (); i++ )
		if ( abs ( pos.x - holes.at(i).Position ().x ) < holes.at(i).Radius () && abs ( pos.y - holes.at(i).Position ().y ) < holes.at(i).Radius () )
			return LevelLost;
	
	if ( abs ( pos.x - Maze::Data ().Goal ().Position ().x ) < Maze::Data ().Goal ().Radius () && abs ( pos.y - Maze::Data ().Goal ().Position ().y ) < Maze::Data ().Goal ().Radius () )
		return LevelComplete;
	
	return GameOK;
}

void Physics::CheckCollision ( CCoord<double> val ) {
	double ballradius = Maze::Data ().BallRadius ();

	if ( val.x < ballradius - 1 ) {
		val.x = 2 * ballradius - val.x - 2;
		speed.x = - ( speed.x * BOUNCE_RED );
		accel.x = 0;
	}
	else if ( val.x > 1 - ballradius ) {
		val.x = 2 * ( 1 - ballradius ) - val.x;
		speed.x = - ( speed.x * BOUNCE_RED );
		accel.x = 0;
	}
	if ( val.y < ballradius - 1 ) {
		val.y = 2 * ballradius - val.y - 2;
		speed.y = - ( speed.y * BOUNCE_RED );
		accel.y = 0;
	}
	else if ( val.y > 1 - ballradius ) {
		val.y = 2 * ( 1 - ballradius ) - val.y;
		speed.y = - ( speed.y * BOUNCE_RED );
		accel.y = 0;
	}

	const vector<CMazeWall> walls = Maze::Data ().Walls ();
	CCoord<double> dist;
	for ( unsigned short i=0; i<walls.size(); i++ ) {
		dist.x = val.x - walls.at(i).Position().x;
		dist.y = val.y - walls.at(i).Position().y;

		if ( abs ( dist.x ) - walls.at(i).HalfSize ().x >= abs ( dist.y ) - walls.at(i).HalfSize ().y ) {
			if ( abs ( dist.x ) < walls.at(i).HalfSize ().x + ballradius ) {
				if ( pos.x <= walls.at(i).Position ().x )
					val.x = 2 * ( walls.at(i).Position ().x - walls.at(i).HalfSize ().x ) - val.x - 2 * ballradius;
				if ( pos.x > walls.at(i).Position ().x )
					val.x = 2 * ( walls.at(i).Position ().x + walls.at(i).HalfSize ().x ) - val.x + 2 * ballradius;

				speed.x = - ( speed.x * BOUNCE_RED );
				accel.x = 0.0f;
			}
		}
		if ( abs ( dist.x ) - walls.at(i).HalfSize ().x <= abs ( dist.y ) - walls.at(i).HalfSize ().y ) {
			if ( abs ( dist.y ) < walls.at(i).HalfSize ().y + ballradius ) {
				if ( pos.y < walls.at(i).Position ().y )
					val.y = 2 * ( walls.at(i).Position ().y - walls.at(i).HalfSize ().y ) - val.y - 2 * ballradius;
				if ( pos.y > walls.at(i).Position ().y )
					val.y = 2 * ( walls.at(i).Position ().y + walls.at(i).HalfSize ().y ) - val.y + 2 * ballradius;

				speed.y = - ( speed.y * BOUNCE_RED );
				accel.y = 0;
			}
		}
	}

	pos.x = val.x;
	pos.y = val.y;
}

CCoord<double> Physics::GetPos() { return pos; }