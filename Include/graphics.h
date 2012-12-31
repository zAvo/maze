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


#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <vector>
#include <fstream>

//#include <GL/glext.h>
//#include <IL/il.h>
//#include <IL/ilu.h>
#include <GL/freeglut.h>
#include <IL/ilut.h>
#include <FTGL/ftgl.h>

#include "header.h"

using namespace std;

class Graphics : IGraphics<Graphics> {
private:
	static int screen_w;
	static int screen_h;
	static float border_w;
	static float border_h;
	static int frame;
	static int t_old;
	static double fps;
	static FTFont *font;
	static FTSimpleLayout *fontlo;
	static GLuint *textures;

	static int EnterGameMode(const char *, char *);
	static void OpenGLInit();
	static void OpenILInit();
	static int LoadTextures();
	static int LoadFont(char *);
	static void DrawMenuBG(bool);

public:
	enum Keys {
		Left		= GLUT_KEY_LEFT,
		Right		= GLUT_KEY_RIGHT,
		Up			= GLUT_KEY_UP,
		Down		= GLUT_KEY_DOWN,
		Enter		= 13,
		Esc			= 27
	};

	static int Init(int *, char **);
	static int Time();
	static void MainLoop();
	static void ExitMainLoop();
	static void PostRedisplay();
	static void SwapBuffers();
	static void DrawRect(CCoord<double>, CCoord<double>, CColor);
	static void DrawCircle(CCoord<double>, double, CColor);
	static void DrawBall(CCoord<double>, double, CColor);
	static void DrawMenu(vector<string>, int, bool, bool);
	static void DrawMaze();
	static void DrawHUD();
	static void DrawScene();
	static void DrawMessage(int, int, char *, ...);
	static void SetFPS();
	static void End();
};

#endif /* _GRAPHICS_H */