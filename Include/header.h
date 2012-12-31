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


#ifndef _HEADER_H
#define _HEADER_H

#include "ifaces.h"
#include "types.h"
#include "xml.h"
#include "graphics.h"
#include "callback.h"
#include "wii.h"
#include "physics.h"
#include "maze.h"
#include "game.h"
#include "enums.h"
#include "menu.h"
#include "locale.h"

using namespace std;

// FreeGLUT window name
#define WINDOW_NAME "Maze"

// XML CONSTS
#define MAX_STR_BUFF 10

// Maze CONSTS
#define MAZE_HOLE_RADIUS 7
#define MAZE_DOT_SIZE 20

// Physics CONSTS
#define REST_VALUE 0.01f
#define BOUNCE_RED 0.53f
#define SPEED_MUL 0.00000000125f

// Texture CONSTS
#define NUM_TEXTURES 5
#define BG_BOARD_TEXT 0
#define BG_MENU_TEXT 1
#define LOGO_TEXT 2
#define ARROW_TEXT 3
#define SPHERE_TEXT 4

// Graphic CONSTS
#define BORDER 0.015f

// Font CONSTS
#define FONT_MULT 0.02f
#define FONT_MULT_MAZES 0.014f

// Radians MACROS
#define DEG_TO_RAD(x) (x * 0.01745329251994329577) // (x * 2 * PI / 360)
#define VAL_TO_RAD(x) (x * 0.78539816339744830966) // (x * PI / 4)

// Font FILES
#define FONT_FILE "../Data/Fonts/wii.ttf"

// Data FILES
#define VIDEOMODELISTFILE "../Data/vmlist.conf"

// Texture FILES
#define BG_BOARD_FILE "../Data/bg_board.jpg"
#define BG_MENU_FILE "../Data/bg_menu.jpg"
#define LOGO_FILE "../Data/logo.png"
#define ARROW_FILE "../Data/arrow.png"
#define SPHERE_FILE "../Data/sphere.png"

// Maze FOLDER & FILES
#define MAZE_FOLDER "../Data/Mazes/"
#define MAZE_XML_SCHEMA "../Data/Mazes/Schema/maze.xsd"


#endif /* _HEADER_H */