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

string CMazeData::name;
CCoord<unsigned short> CMazeData::size;
CCoord<double> CMazeData::start;
int CMazeData::time;
CMazeBall CMazeData::ball;
CMazeHole CMazeData::goal;
vector<CMazeWall> CMazeData::walls;
vector<CMazeHole> CMazeData::holes;
CMazeData Maze::data;

string CMazeData::Name() { return name; }
CCoord<unsigned short> CMazeData::Size() { return size; }
CCoord<double> CMazeData::Start() { return start; }
CMazeHole CMazeData::Goal() { return goal; }
int CMazeData::Time() { return time; }
double CMazeData::BallSpeed() { return ball.Speed(); }
double CMazeData::BallRadius() { return ball.Radius(); }


int Maze::Load(const char *filename) {
	vector<string> mazedata;
	CCoord<double> pos, hsiz;
	CMazeWall wall;
	CMazeHole hole;
	double coord[4];
	double rad;
	int wnumber, hnumber, number, tmp;

	string fname(MAZE_FOLDER);
	fname.append(filename);
	
	if (XML::GetMazeData(fname.c_str(), &mazedata) != 0)
		return -1;

	// name
	CMazeData::name.assign(mazedata[0].c_str());

	// time
	sscanf_s(mazedata[1].c_str(), "%d", &CMazeData::time);
	CMazeData::time *= 1000;

	// size
	sscanf_s(mazedata[2].c_str(), "%d", &CMazeData::size.x);
	sscanf_s(mazedata[3].c_str(), "%d", &CMazeData::size.y);

	// ball set
	double radius, speed;
	radius = ((double)MAZE_DOT_SIZE) / (CMazeData::size.x * 2);
	speed  = SPEED_MUL * (4196 - CMazeData::size.x);
	CMazeData::ball.Init(radius, speed);

	// start
	sscanf_s(mazedata[5].c_str(), "%d", &tmp);
	CMazeData::start.x = ((double)tmp * 2 / CMazeData::size.x) - 1;
	sscanf_s(mazedata[6].c_str(), "%d", &tmp);
	CMazeData::start.y = 1 - ((double)tmp * 2 / CMazeData::size.y);

	// goal
	sscanf_s(mazedata[7].c_str(), "%d", &tmp);
	pos.x = ((double)tmp * 2 / CMazeData::size.x) - 1;
	sscanf_s(mazedata[8].c_str(), "%d", &tmp);
	pos.y = 1 - ((double)tmp * 2 / CMazeData::size.y);
	sscanf_s(mazedata[9].c_str(), "%d", &tmp);
	rad = (double)tmp * 2 / CMazeData::size.x;

	CMazeData::goal.Init(pos, rad);

	// walls
	int i, j = 10;
	sscanf_s(mazedata[j++].c_str(), "%d", &wnumber);
	for (i=0; i<wnumber; i+=1) {
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		coord[0] = (double)tmp;
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		coord[1] = (double)tmp;
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		coord[2] = (double)tmp;
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		coord[3] = (double)tmp;

		pos.x = ((coord[0] + coord[2]) / CMazeData::size.x) - 1;
		pos.y = 1 - ((coord[1] + coord[3]) / CMazeData::size.y);
		hsiz.x = (coord[2] - coord[0]) / CMazeData::size.x;
		hsiz.y = (coord[3] - coord[1]) / CMazeData::size.y;

		wall.Init(pos, hsiz);
		CMazeData::walls.push_back(wall);
	}

	// holes
	sscanf_s(mazedata[j++].c_str(), "%d", &hnumber);
	for (i=0; i<hnumber; i+=1) {
		sscanf_s(mazedata[j++].c_str(), "%d", &number);
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		pos.x = ((double)tmp * 2 / CMazeData::size.x) - 1;
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		pos.y = 1 - ((double)tmp * 2 / CMazeData::size.y);
		sscanf_s(mazedata[j++].c_str(), "%d", &tmp);
		rad = (double)tmp * 2 / CMazeData::size.x;

		hole.Init(pos, rad, number);
		CMazeData::holes.push_back(hole);
	}

	return 0;
}

int Maze::Time() { return data.time; }

void Maze::Draw() {
	CColor wallc(0.0f, 0.0f, 0.0f, 1.0f);
	CColor goalc(1.0f, 0.0f, 0.0f, 1.0f);
	CColor ballc(0.5f, 0.5f, 0.5f, 1.0f);

	// walls
	for (unsigned int i=0; i<CMazeData::walls.size(); i++) {
		Graphics::DrawRect(CMazeData::walls[i].Position(), CMazeData::walls[i].HalfSize(), wallc);
	}

	// holes
	for (unsigned int i=0; i<CMazeData::holes.size(); i++) {
		Graphics::DrawCircle(CMazeData::holes[i].Position(), CMazeData::holes[i].Radius(), wallc);
	}

	// goal
	Graphics::DrawCircle(CMazeData::goal.Position(), CMazeData::goal.Radius(), goalc);

	// ball
	Graphics::DrawBall(Physics::GetPos(), CMazeData::ball.Radius(), ballc);
}

CMazeData Maze::Data() { return data; }

void Maze::Free() {
	data.walls.clear();
	data.holes.clear();
}

Maze::~Maze() {}