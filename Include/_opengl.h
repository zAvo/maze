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
