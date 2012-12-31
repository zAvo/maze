#include "../Include/header.h"

int Callback::frame;
int Callback::time;
int Callback::timebase;
short Callback::game_status;

void Callback::SetGameStatus ( short status ) { game_status = status; }
short Callback::GetGameStatus () { return game_status; }


void Callback::RenderScene () {
	OpenGL::DrawBackground ();
	OpenGL::DrawMaze ();

	switch ( game_status ) {
		case GAME_OK:
			OpenGL::DrawDot ();
			OpenGL::DrawHUD ();
			break;
		case GAME_WB_OK:
			OpenGL::DrawHUD ();
			break;
		case GAME_WM_OK:
			OpenGL::DrawHUD ();
			break;
		case GAME_WM_NO:
			OpenGL::DrawHUD ();
			break;
		case GAME_LEVEL_COMPLETE:
			OpenGL::LevelComplete ();
			break;
		case GAME_OVER:
			OpenGL::GameOver ();
			break;
		case GAME_TIME_IS_UP:
			OpenGL::TimeIsUp ();
			break;
	}

	glutSwapBuffers ();
}

void Callback::ProcessNormalKeys ( unsigned char key, int x, int y ) {
	switch ( key ) {
		case 13:
			if ( game_status >= GAME_WB_OK )
				OpenGL::StartLevel ();
			break;
		case 27:
			exit ( 0 );
			break;
	}
}


void Callback::PressKey ( int key, int x, int y ) {
	switch (key) {
		case GLUT_KEY_F1 :
			if ( game_status == GAME_WM_NO )
				Wii::Init ();
			break;
	}
}

void Callback::Idle () {
	OpenGL::SetFPS ();
	switch ( game_status ) {
		case GAME_OK:
			Wii::Poll ();
			Physics::ComputePosition ();
			break;
		case GAME_WM_OK:
		case GAME_WB_OK:
			Wii::Poll ();
			break;
	}
	glutPostRedisplay();
}