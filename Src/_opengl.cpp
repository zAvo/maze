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

GLuint *OpenGL::textures;
string OpenGL::activeGameMode;
int OpenGL::window_width;
int OpenGL::window_height;
double OpenGL::screen_offset_x;
int OpenGL::frame;
int OpenGL::time;
int OpenGL::time_start;
int OpenGL::time_level;
int OpenGL::timebase;
vector<unsigned short> OpenGL::mazepoints;
string OpenGL::mazename;
unsigned short OpenGL::mazesize;
double OpenGL::dotradius;
double OpenGL::holeradius;
double OpenGL::borderwidth;
FTFont *OpenGL::font;
FTSimpleLayout *OpenGL::fontlo;
char OpenGL::fps_str[15];

int OpenGL::Time () { return glutGet ( GLUT_ELAPSED_TIME ); }
unsigned short OpenGL::GetMazeSize () { return mazesize; }

OpenGL::OpenGL ( int argc, char *argv[] ){
	char temp[20];

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL );

	ifstream listfile ( GAMEMODELISTFILE );

	//if ( !listfile.is_open () );

	while ( listfile.good () ) {
		listfile.getline ( temp, 20 );
		glutGameModeString ( temp );
		if ( glutGameModeGet ( GLUT_GAME_MODE_POSSIBLE ) ) {
			activeGameMode = temp;
			gameModeList.push_back ( (string) temp );
		}
			
	}

	listfile.close();
}


OpenGL::~OpenGL () {
	glFinish ();
}

void OpenGL::ILInit () {
	ilInit ();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
}

int OpenGL::GameMode () {
//	glutGameModeString ( "1280x1024:32@60" );
	glutGameModeString ( activeGameMode.c_str () );
	if ( !glutGameModeGet ( GLUT_GAME_MODE_POSSIBLE ) )
		return 1;

	glutEnterGameMode ();
	glutSetCursor ( GLUT_CURSOR_NONE );
	//glutSetCursor ( GLUT_CURSOR_INHERIT );

	Init ();

	return 0;
}

void OpenGL::Init () {
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glutKeyboardFunc( Callback::ProcessNormalKeys );
	glutSpecialFunc( Callback::PressKey );
//	glutSpecialUpFunc(releaseKey);
	glutDisplayFunc( (Callback::RenderScene) );
	glutIdleFunc( (Callback::Idle) );
//	glutReshapeFunc(changeSize);
//	initScene();

	window_width = glutGet ( GLUT_WINDOW_WIDTH );
	window_height = glutGet ( GLUT_WINDOW_HEIGHT );
	borderwidth = BORDER;

	ILInit ();
	MazeInit ( MAZE_FILE );

	LoadTextures ();

	LoadFont ( FONT_FILE );

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	screen_offset_x = ( ( ( double ) window_width / ( double ) window_height ) - 1.0f ) * 2;
	gluOrtho2D ( -1.0f - screen_offset_x - ( 3 * borderwidth ), 1 + ( 3 * borderwidth ), -1.0f - ( 3 * borderwidth ), 1.0f + ( 3 * borderwidth ) );

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
}

void OpenGL::MainLoop () {
	glutMainLoop ();
}

void OpenGL::RenderBitmapString ( double x, double y, void *font,char *string )
{
	int len, i;
	glRasterPos2d ( x, y );
	len = (int) strlen ( string );
	for ( i = 0; i < len; i++ )
	{
		glutBitmapCharacter ( font, string[i] );
	}
}

void OpenGL::DrawSquare ( double x, double y, unsigned short halfside ) {
	glColor3f ( 0.0f, 0.0f, 0.0f );
    glBegin ( GL_QUADS );
            glVertex2d ( x-halfside, y-halfside );
            glVertex2d ( x+halfside, y-halfside );
            glVertex2d ( x+halfside, y+halfside );
            glVertex2d ( x-halfside, y+halfside );
    glEnd ();
}

void OpenGL::DrawDot () {
	glPushMatrix ();
		glColor3f ( 0.0f, 0.0f, 0.0f );
		glTranslated ( Physics::GetX (), Physics::GetY (), 0 );
		DrawCircle ( dotradius );
	glPopMatrix ();
}

void OpenGL::LoadTextures () {
	textures = new GLuint ( NUM_TEXTURES );
	textures[BOARD_BG_TEXT] = ilutGLLoadImage ( BOARD_BG_FILE );
	ILenum ErrorIL;
	while ((ErrorIL = ilGetError()) != IL_NO_ERROR)
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));

	textures[INFO_BG_TEXT] = ilutGLLoadImage ( INFO_BG_FILE );
	while ((ErrorIL = ilGetError()) != IL_NO_ERROR)
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));

	textures[LOGO_TEXT] = ilutGLLoadImage ( LOGO_FILE );
	while ((ErrorIL = ilGetError()) != IL_NO_ERROR)
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
}

void OpenGL::DrawBackground () {
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		glEnable (GL_TEXTURE_2D);
				glColor3f ( 1.0f, 1.0f, 1.0f );

				glBindTexture(GL_TEXTURE_2D, textures[INFO_BG_TEXT]);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2d(-1.0f-screen_offset_x, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex2d(-1.0f-(3*borderwidth), 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex2d(-1.0f-(3*borderwidth), -1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex2d(-1.0f-screen_offset_x, -1.0f);
				glEnd();

				glColor3f ( 1.0f, 1.0f, 0.0f );

				glBindTexture(GL_TEXTURE_2D, textures[BOARD_BG_TEXT]);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2d(-1.0f, -1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex2d(1.0f, -1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex2d(1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex2d(-1.0f, 1.0f);
				glEnd();

				glPushMatrix ();
				glTranslated ( -screen_offset_x*0.5-1.5*borderwidth, 0.0f, 0.0f );
					glBindTexture(GL_TEXTURE_2D, textures[LOGO_TEXT]);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 1.0f); glVertex2d(-1.3f, 0.8f);
						glTexCoord2f(1.0f, 1.0f); glVertex2d(-0.7f, 0.8f);
						glTexCoord2f(1.0f, 0.0f); glVertex2d(-0.7f, 0.6f);
						glTexCoord2f(0.0f, 0.0f); glVertex2d(-1.3f, 0.6f);
					glEnd();
				glPopMatrix ();
		glDisable (GL_TEXTURE_2D);
	glDisable ( GL_BLEND );
}

void OpenGL::MazeInit ( char *filename ) {
	ifstream mazefile ( filename );

	//if ( !mazefile.is_open () );

	unsigned short tmp;
	unsigned short walls;
	char tmpstr[100];
	if ( mazefile.good () )
		mazefile >> tmpstr;
	if ( mazefile.good () )
		mazefile >> time_level;
	if ( mazefile.good () )
		mazefile >> mazesize;
	if ( mazefile.good () )
		mazefile >> walls;

	while ( mazefile.good () ) {
		mazefile >> tmp;
		mazepoints.push_back ( tmp );
	}

	mazefile.close ();

	mazename.assign ( tmpstr );
	dotradius = ( (double) MAZE_DOT_SIZE ) / ( mazesize * 2 );
	holeradius = ((double) MAZE_HOLE_RADIUS) / mazesize;
	borderwidth = BORDER;
	
	time_start = -1;
	time_level *= 1000;
}
/*
void OpenGL::DrawMazeBorders () {
	glColor3f ( 0.0f, 0.0f, 0.0f );

	glBegin ( GL_QUADS );
        glVertex2d ( -1.0f, -1.0f - borderwidth );
        glVertex2d ( 1.0f + borderwidth, -1.0f - borderwidth );
		glVertex2d ( 1.0f + borderwidth, -1.0f );
		glVertex2d ( -1.0f, -1.0f );
	glEnd ();
	glBegin ( GL_QUADS );
        glVertex2d ( 1.0f, -1.0f );
        glVertex2d ( 1.0f + borderwidth, -1.0f );
		glVertex2d ( 1.0f + borderwidth, 1.0f + borderwidth );
		glVertex2d ( 1.0f, 1.0f + borderwidth );
	glEnd ();
	glBegin ( GL_QUADS );
        glVertex2d ( -1.0f - borderwidth, 1.0f );
        glVertex2d ( 1.0f, 1.0f );
		glVertex2d ( 1.0f, 1.0f + borderwidth );
		glVertex2d ( -1.0f - borderwidth, 1.0f + borderwidth );
	glEnd ();
	glBegin ( GL_QUADS );
        glVertex2d ( -1.0f - borderwidth, -1.0f - borderwidth );
        glVertex2d ( -1.0f, -1.0f - borderwidth );
		glVertex2d ( -1.0f, 1.0f );
		glVertex2d ( -1.0f - borderwidth, 1.0f );
	glEnd ();
}
*/
void OpenGL::DrawMazeWalls () {
	glColor3f ( 0.0f, 0.0f, 0.0f );

	const vector<CMazeWall> walls = Maze::Data ().Walls ();

	for ( unsigned int i=0; i<walls.size (); i++) {
		glBegin ( GL_QUADS );
			glVertex2d ( walls.at(i).Position ().x - walls.at(i).HalfSize ().x, walls.at(i).Position ().y - walls.at(i).HalfSize ().y );
			glVertex2d ( walls.at(i).Position ().x + walls.at(i).HalfSize ().x, walls.at(i).Position ().y - walls.at(i).HalfSize ().y );
			glVertex2d ( walls.at(i).Position ().x + walls.at(i).HalfSize ().x, walls.at(i).Position ().y + walls.at(i).HalfSize ().y );
			glVertex2d ( walls.at(i).Position ().x - walls.at(i).HalfSize ().x, walls.at(i).Position ().y + walls.at(i).HalfSize ().y );
		glEnd ();
	}
}

void OpenGL::DrawMazeHoles () {
	glColor3f ( 0.0f, 0.0f, 0.0f );

	unsigned int i;
	const vector<CMazeHole> holes = Maze::Data ().Holes ();
	for ( i=0; i<holes.size (); i++ ) {
		glPushMatrix ();
		glTranslated ( holes.at(i).Position ().x, holes.at(i).Position ().y, 0);
			DrawCircle ( holes.at(i).Radius () );
		glPopMatrix ();
	}

	glColor3f ( 1.0f, 0.0f, 0.0f );
	glPushMatrix ();
	glTranslated ( Maze::Data ().Goal ().Position ().x, Maze::Data ().Goal ().Position ().y, 0);
		DrawCircle ( Maze::Data ().Goal ().Radius () );
	glPopMatrix ();
}


void OpenGL::DrawMaze () {
	DrawMazeWalls ();

	DrawMazeHoles ();
}

void OpenGL::DrawCircle ( double radius ) {
	glBegin(GL_POLYGON);
		for (unsigned int i=0; i<360; i++) {
			double rad = DEG_TO_RAD ( i );
			glVertex2f ( (GLfloat) ( cos ( rad ) * radius ), (GLfloat) ( sin ( rad ) * radius ) );
		}
	glEnd();
}

void OpenGL::LoadFont ( char *font_file ) {
	// Create a pixmap font from a TrueType file.
	font = new FTGLTextureFont ( font_file );
	// If something went wrong, bail out.
	if ( font->Error () )
		printf ( "%d\n", font->Error () );

	font->FaceSize ( (unsigned int) (window_width * FONT_MULT) );
	if ( font->Error () )
		printf ( "%d\n", font->Error () );

	font->CharMap ( ft_encoding_unicode );
	if ( font->Error () )
		printf ( "%d\n", font->Error () );

	fontlo = new FTSimpleLayout ();
	if ( fontlo->Error () )
		printf ( "%d\n", fontlo->Error () );

	fontlo->SetFont ( font );
	fontlo->SetLineLength ( (float) window_width - window_height );
	if ( fontlo->Error () )
		printf ( "%d\n", fontlo->Error () );
	fontlo->SetAlignment ( FTGL::ALIGN_CENTER );
	if ( fontlo->Error () )
		printf ( "%d\n", fontlo->Error () );
}

void OpenGL::SetFPS () {
	frame++;
	time = glutGet ( GLUT_ELAPSED_TIME );
	if ( time - timebase > 1000 ) {
		double fps = frame * 1000.0 / (time - timebase);
		timebase = time;		
		frame = 0;

		sprintf ( fps_str, "FPS: %4.2f", fps );
	}
}

void OpenGL::DrawHUD () {
	char wbdata[45];
//	char wbweight[30];
//	double x, y, weight;
	double x, y;

	x = Physics::GetX ();
	y = Physics::GetY ();
//	weight = Wii::GetWeight ();

	sprintf_s ( wbdata, "x: %f, y: %f", x, y );
//	sprintf_s ( wbweight, "Weight: %f", weight );

	glColor3f (1.0f, 1.0f, 0.0f );
	RenderBitmapString ( -1.6f, 0.95f, BITMAP_FONT, wbdata );




	double width_mult = 1.0f / window_width;

	glColor3f ( 0.0f, 0.0f, 0.0f );

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D ( 0, window_width, 0, window_height );

		glMatrixMode ( GL_MODELVIEW );
			glPushMatrix ();
				glTranslated ( 0, 0.05*window_height, 0 );
				fontlo->Render ( fps_str );
			
				glLoadIdentity ();
				glTranslated ( 0, 0.7*window_height, 0 );
				fontlo->Render ( "LEVEL:" );
				
				glLoadIdentity ();
				glTranslated ( 0, 0.65*window_height, 0 );
				fontlo->Render ( mazename.c_str () );

				if ( time_start == -1 ) {
					glLoadIdentity ();
					glTranslated ( 0, 0.45*window_height, 0 );
					fontlo->Render ( "Press F1 to start" );
				glPopMatrix ();
				}
				else {
					glLoadIdentity ();
					glTranslated ( 0, 0.55*window_height, 0 );
					fontlo->Render ( "TIME LEFT:" );

					int scrleftsz = window_width - window_height;
					int elapsed = time - time_start;
					double timesiz = (borderwidth+0.021f)*window_width + (( 1 - (double) elapsed / time_level ) * 0.89f * (scrleftsz - (borderwidth+0.021f)*window_width));

					int secs = ( int ) ( ( time_level - elapsed ) * 0.001 );
					char tmp_str[8];
					sprintf ( tmp_str, "%dm %ds", secs / 60, secs % 60 );

					glLoadIdentity ();
					glTranslated ( 0, 0.5*window_height, 0 );
					fontlo->Render ( tmp_str );

					glLoadIdentity ();
					glBegin ( GL_LINE_STRIP );
						glVertex2d ( (borderwidth+0.02f)*window_width, 0.37f*window_height );
						glVertex2d ( scrleftsz-(borderwidth+0.02f)*window_width, 0.37f*window_height );
						glVertex2d ( scrleftsz-(borderwidth+0.02f)*window_width, 0.34f*window_height );
						glVertex2d ( (borderwidth+0.02f)*window_width, 0.34f*window_height );
						glVertex2d ( (borderwidth+0.02f)*window_width, 0.37f*window_height );
					glEnd ();

					glBegin ( GL_QUADS );
						glVertex2d ( (borderwidth+0.021f)*window_width, 0.368f*window_height );
						glVertex2d ( timesiz, 0.368f*window_height );
						glVertex2d ( timesiz, 0.343f*window_height );
						glVertex2d ( (borderwidth+0.021f)*window_width, 0.343f*window_height );
					glEnd ();

					if ( elapsed >= time_level )
						Callback::SetGameStatus ( GAME_TIME_IS_UP );
				}
		glPopMatrix ();
		
		glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();

	glMatrixMode ( GL_MODELVIEW );
}

void OpenGL::LevelComplete () {
	glColor3f ( 0.0f, 0.0f, 1.0f );

	font->FaceSize ( ( unsigned int ) ( window_width * FONT_MULT * 8 ) );
	fontlo->SetLineLength ( (float) window_width );

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D ( 0, window_width, 0, window_height );

		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix ();
			glTranslated ( 0, 0.4*window_height, 0 );
			fontlo->Render ( "LEVEL COMPLETE!" );
		glPopMatrix ();

		glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();

	glMatrixMode ( GL_MODELVIEW );
}

void OpenGL::GameOver () {
	glColor3f ( 0.0f, 0.0f, 1.0f );

	font->FaceSize ( ( unsigned int ) ( window_width * FONT_MULT * 8 ) );
	fontlo->SetLineLength ( (float) window_width );

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D ( 0, window_width, 0, window_height );

		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix ();
			glTranslated ( 0, 0.4*window_height, 0 );
			fontlo->Render ( "GAME OVER!" );
		glPopMatrix ();

		glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();

	glMatrixMode ( GL_MODELVIEW );
}

void OpenGL::TimeIsUp () {
	glColor3f ( 0.0f, 0.0f, 1.0f );

	font->FaceSize ( ( unsigned int ) ( window_width * FONT_MULT * 8 ) );
	fontlo->SetLineLength ( (float) window_width );

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D ( 0, window_width, 0, window_height );

		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix ();
			glTranslated ( 0, 0.4*window_height, 0 );
			fontlo->Render ( "TIME IS UP!" );
		glPopMatrix ();

		glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();

	glMatrixMode ( GL_MODELVIEW );
}

void OpenGL::SetTimeStart () {
	time_start = glutGet ( GLUT_ELAPSED_TIME );
}

void OpenGL::StartLevel () {

	font->FaceSize ( ( unsigned int ) ( window_width * FONT_MULT ) );
	fontlo->SetLineLength ( (float) (window_width - window_height) );

	SetTimeStart ();
	Callback::SetGameStatus ( GAME_OK );
	Physics::Init ();
}