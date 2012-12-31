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


#ifndef _MAZE_IFACES_H
#define _MAZE_IFACES_H

#include "header.h"

using namespace std;


// IGame interface
template <typename T>
class IGame {
public:
	IGame() {
		static enum Status (*InitCheck)(int *, char **) = T::Init;
		static void (*DrawSceneCheck)() = T::DrawScene;
		static void (*IdleCheck)() = T::Idle;
		static int (*TimeLeftCheck)() = T::TimeLeft;
		static void (*ProcessNormalKeysCheck)(unsigned char, int, int) = T::ProcessNormalKeys;
		static void (*ProcessSpecialKeysCheck)(int, int, int) = T::ProcessSpecialKeys;
	}
};

// IGraphics interface
template <typename T>
class IGraphics {
public:
	IGraphics() {
		static int (*InitCheck)(int *, char **) = T::Init;
		static int (*TimeCheck)() = T::Time;
		static void (*MainLoopCheck)() = T::MainLoop;
		static void (*ExitMainLoopCheck)() = T::ExitMainLoop;
		static void (*PostRedisplayCheck)() = T::PostRedisplay;
		static void (*SwapBuffersCheck)() = T::SwapBuffers;
		static void (*DrawRectCheck)(CCoord<double>, CCoord<double>, CColor) = T::DrawRect;
		static void (*DrawCircleCheck)(CCoord<double>, double, CColor) = T::DrawCircle;
		static void (*DrawBallCheck)(CCoord<double>, double, CColor) = T::DrawBall;
		static void (*DrawMenuCheck)(vector<string>, int, bool, bool) = T::DrawMenu;
		static void (*DrawMazeCheck)() = T::DrawMaze;
		static void (*DrawHUDCheck)() = T::DrawHUD;
		static void (*DrawSceneCheck)() = T::DrawScene;
		static void (*DrawMessageCheck)(int, int, char *, ...) = T::DrawMessage;
		static void (*SetFPSCheck)() = T::SetFPS;
		static void (*EndCheck)() = T::End;
	}
};

// ILocale interface
template <typename T>
class ILocale {
public:
	ILocale() {
		static int (*GetFileListCheck)(const char *, vector<string> *, vector<string> *) = T::GetFileList;
	}
};

#endif /* _MAZE_IFACES_H */