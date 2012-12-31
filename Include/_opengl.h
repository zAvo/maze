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


#ifndef _OPENGL_H
#define _OPENGL_H

#include "header.h"

using namespace std;

class OpenGL {
private:
	static GLuint *textures;
	static string activeGameMode;
	static int window_width;
	static int window_height;
	static double screen_offset_x;
	vector<string> gameModeList;
	static int frame;
	static int time;
	static int time_start;
	static int time_level;
	static int timebase;
	static vector<unsigned short> mazepoints;
	static string mazename;
	static unsigned short mazesize;
	static double dotradius;
	static double holeradius;
	static double borderwidth;
	static FTFont *font;
	static FTSimpleLayout *fontlo;
	static char fps_str[15];

	static void Init ();
	static void ILInit ();
	static void DrawRect ( vector<int>, int );
	static void DrawCircle ( double radius );
	static void MazeInit ( char * );
	static void DrawMazeWalls ();
	static void DrawMazeHoles ();
	static void MazeInitMatrix ();
	static void LoadFont ( char * );
	static void LoadTextures ();
	static void DrawSquare ( double, double, unsigned short );
public:
	OpenGL ( int, char ** );
	~OpenGL ();

	static int GameMode ();
	static void MainLoop ();
	static void SetFPS ();
	static int Time ();
	static void DrawBackground ();
	static void DrawMaze ();
	static void DrawDot ();
	static void SetTimeStart ();
	static void DrawHUD ();
	static void GameOver ();
	static void TimeIsUp ();
	static void LevelComplete ();
	static void StartLevel ();
	static void RenderBitmapString ( double, double, void *,char * );
	//static GLuint loadPNGTexture (const char *);
	static unsigned short GetMazeSize ();
};

#endif /* _OPENGL_H */
