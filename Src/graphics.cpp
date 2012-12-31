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


int Graphics::screen_w;
int Graphics::screen_h;
float Graphics::border_w;
float Graphics::border_h;
int Graphics::frame;
int Graphics::t_old;
double Graphics::fps;
FTSimpleLayout *Graphics::fontlo;
FTFont *Graphics::font;
GLuint *Graphics::textures;

// IFACES

void Graphics::MainLoop() {
	glutMainLoop();
}

void Graphics::ExitMainLoop() {
	glutLeaveMainLoop();
}

void Graphics::PostRedisplay() {
	glutPostRedisplay();
}

void Graphics::SwapBuffers() {
	glutSwapBuffers();
}

void Graphics::SetFPS() {
	frame++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - t_old > 1000) {
		fps = frame * 1000.0 / (time - t_old);
		t_old = time;		
		frame = 0;
	}
}

void Graphics::DrawCircle(CCoord<double> pos, double radius, CColor col) {
	glPushMatrix();
		glTranslated(pos.x, pos.y, 0);

		glColor4d(col.r, col.g, col.b, col.a);
		glBegin(GL_POLYGON);
			for (unsigned int i=0; i<360; i+=4) {
				double rad = DEG_TO_RAD(i);
				glVertex2f((GLfloat)(cos(rad) * radius), (GLfloat)(sin(rad) * radius));
			}
		glEnd();
	glPopMatrix();
}

void Graphics::DrawRect (CCoord<double> pos, CCoord<double> hs, CColor col) {
	glColor4d(col.r, col.g, col.b, col.a);
	glBegin(GL_QUADS);
		glVertex2d(pos.x-hs.x, pos.y+hs.y); // top left
		glVertex2d(pos.x+hs.x, pos.y+hs.y); // top right
		glVertex2d(pos.x+hs.x, pos.y-hs.y); // bottom right
		glVertex2d(pos.x-hs.x, pos.y-hs.y); // bottom left
	glEnd();
}

void Graphics::DrawBall (CCoord<double> pos, double radius, CColor col) {
	glColor4d(col.r, col.g, col.b, col.a);

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[SPHERE_TEXT]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2d(pos.x-radius, pos.y+radius); // top left
			glTexCoord2f(1.0f, 1.0f); glVertex2d(pos.x+radius, pos.y+radius); // top right
			glTexCoord2f(1.0f, 0.0f); glVertex2d(pos.x+radius, pos.y-radius); // bottom right
			glTexCoord2f(0.0f, 0.0f); glVertex2d(pos.x-radius, pos.y-radius); // bottom left
		glEnd();
}

int Graphics::Time() {
	return glutGet(GLUT_ELAPSED_TIME);
}

int Graphics::Init(int *argc, char *argv[]) {
	char temp[20];
	vector<string> gameModeList;
	GLenum error;

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	ifstream listfile(VIDEOMODELISTFILE);
	if (!listfile.is_open())
		return 1;

	while (listfile.good()) {
		listfile.getline(temp, 20);
		glutGameModeString(temp);
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			gameModeList.push_back((string)temp);
		}
	}

	listfile.close();

	if (EnterGameMode(gameModeList.back().c_str(), WINDOW_NAME) != 0)
		return -1;

	OpenGLInit();

	while ((error = glGetError()) != GL_NO_ERROR)
		printf("GLERROR: %s", gluErrorString(error));

	screen_w = glutGet(GLUT_WINDOW_WIDTH);
	screen_h = glutGet(GLUT_WINDOW_HEIGHT);
	border_w = BORDER * screen_h / screen_w;
	border_h = BORDER;

	OpenILInit();

	if (LoadTextures() != 0)
		return -2;

	if (LoadFont(FONT_FILE) != 0)
		return -3;

	/********* PROJECTIONS **********/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0f - 3 * border_w, 1.0f + 3 * border_w, -1.0f - 3 * border_h, 1.0f + 3 * border_h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	frame = 0;
	t_old = glutGet(GLUT_ELAPSED_TIME);

	return 0;
}

int Graphics::LoadFont(char *font_file) {
	// Create a pixmap font from a TrueType file.
	font = new FTGLTextureFont(font_file);
	// If something went wrong, bail out.
	if (font->Error()) {
		printf("%d\n", font->Error());
		return 1;
	}

	font->FaceSize((unsigned int)(screen_w * FONT_MULT));
	if (font->Error()) {
		printf("%d\n", font->Error());
		return 1;
	}

	font->CharMap(ft_encoding_unicode);
	if (font->Error()) {
		printf("%d\n", font->Error());
		return 1;
	}

	fontlo = new FTSimpleLayout();
	if (fontlo->Error()) {
		printf("%d\n", fontlo->Error());
		return 1;
	}

	fontlo->SetFont(font);
	if (fontlo->Error()) {
		printf("%d\n", fontlo->Error());
		return 1;
	}

	fontlo->SetLineLength((float)(screen_w));
	if (fontlo->Error()) {
		printf("%d\n", fontlo->Error());
		return 1;
	}

	fontlo->SetAlignment(FTGL::ALIGN_CENTER);
	if (fontlo->Error()) {
		printf("%d\n", fontlo->Error());
		return 1;
	}

	return 0;
}

int Graphics::LoadTextures() {
	textures = new GLuint(NUM_TEXTURES);
	textures[BG_BOARD_TEXT] = ilutGLLoadImage(BG_BOARD_FILE);
	ILenum ErrorIL;
	if ((ErrorIL = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
		return 1;
	}

	textures[BG_MENU_TEXT] = ilutGLLoadImage(BG_MENU_FILE);
	if ((ErrorIL = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
		return 1;
	}

	textures[LOGO_TEXT] = ilutGLLoadImage(LOGO_FILE);
	if ((ErrorIL = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
		return 1;
	}

	textures[ARROW_TEXT] = ilutGLLoadImage(ARROW_FILE);
	if ((ErrorIL = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
		return 1;
	}

	textures[SPHERE_TEXT] = ilutGLLoadImage(SPHERE_FILE);
	if ((ErrorIL = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", ErrorIL, iluErrorString(ErrorIL));
		return 1;
	}

	return 0;
}

void Graphics::OpenILInit() {
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
}

void Graphics::OpenGLInit() {
	glutSetCursor(GLUT_CURSOR_NONE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutKeyboardFunc(Game::ProcessNormalKeys);
	glutSpecialFunc(Game::ProcessSpecialKeys);
	glutDisplayFunc(Game::DrawScene);
	glutIdleFunc(Game::Idle);
}

int Graphics::EnterGameMode(const char *mode, char *wName) {
	glutGameModeString(mode);
	if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
		return -1;

	glutEnterGameMode(wName);

	return 0;
}

void Graphics::DrawMenuBG(bool isGame) {
	GLenum error;

	if (!isGame)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	while ((error = glGetError()) != GL_NO_ERROR)
		printf("GLERROR: %s", gluErrorString(error));

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		while ((error = glGetError()) != GL_NO_ERROR)
		printf("GLERROR: %s", gluErrorString(error));

		glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
		glBegin(GL_QUADS);
			glVertex2d(-1.0f, 1.0f);
			glVertex2d(1.0f, 1.0f);
			glVertex2d(1.0f, -1.0f);
			glVertex2d(-1.0f, -1.0f);
		glEnd();

		while ((error = glGetError()) != GL_NO_ERROR)
			printf("GLERROR: %s", gluErrorString(error));

		glEnable(GL_TEXTURE_2D);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glBindTexture(GL_TEXTURE_2D, textures[BG_MENU_TEXT]);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2d(-0.45f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2d(0.45f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2d(0.45f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex2d(-0.45f, -1.0f);
			glEnd();

			while ((error = glGetError()) != GL_NO_ERROR)
				printf("GLERROR: %s", gluErrorString(error));

			glBindTexture(GL_TEXTURE_2D, textures[LOGO_TEXT]);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2d(-0.23f, 0.8f);
				glTexCoord2f(1.0f, 1.0f); glVertex2d(0.23f, 0.8f);
				glTexCoord2f(1.0f, 0.0f); glVertex2d(0.23f, 0.6f);
				glTexCoord2f(0.0f, 0.0f); glVertex2d(-0.23f, 0.6f);
			glEnd();

			while ((error = glGetError()) != GL_NO_ERROR)
				printf("GLERROR: %s", gluErrorString(error));
		
		glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Graphics::DrawMenu(vector<string> entryList, int active, bool isLevels, bool isGame) {
	GLenum error;

	DrawMenuBG(isGame);

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float translateEntry;
		int min = 0, max = entryList.size() - 1;

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		if (isLevels) {
			translateEntry = -0.05f;

			if (entryList.size() >= 10) {
				min = active - 4;
				if (10 + min > (int)entryList.size())
					min = entryList.size() - 10;
				if (min <= 0)
					min = 0;
				else {
					glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, textures[ARROW_TEXT]);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 1.0f); glVertex2d(-0.035f, 0.45f);
							glTexCoord2f(1.0f, 1.0f); glVertex2d(0.035f, 0.45f);
							glTexCoord2f(1.0f, 0.0f); glVertex2d(0.035f, 0.39f);
							glTexCoord2f(0.0f, 0.0f); glVertex2d(-0.035f, 0.39f);
						glEnd();
					glDisable(GL_TEXTURE_2D);

					while ((error = glGetError()) != GL_NO_ERROR)
						printf("GLERROR: %s", gluErrorString(error));
				}

				max = min + 9;
				if (max >= (int)entryList.size() - 1)
					max = entryList.size() - 1;
				else {
					glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, textures[ARROW_TEXT]);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 1.0f); glVertex2d(-0.035f, -0.63f);
							glTexCoord2f(1.0f, 1.0f); glVertex2d(0.035f, -0.63f);
							glTexCoord2f(1.0f, 0.0f); glVertex2d(0.035f, -0.57f);
							glTexCoord2f(0.0f, 0.0f); glVertex2d(-0.035f, -0.57f);
						glEnd();
					glDisable(GL_TEXTURE_2D);

					while ((error = glGetError()) != GL_NO_ERROR)
						printf("GLERROR: %s", gluErrorString(error));
				}
			}
		}
		else translateEntry = -0.15f;

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, screen_w, 0, screen_h);

		glTranslated(0, screen_h*0.65f, 0);

		for (int i=min; i<max; i++) {
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			if (isLevels) {
				if (i == active - 2 || i == active + 2)
					font->FaceSize((unsigned int)(screen_w * FONT_MULT_MAZES * 1.075));
				else if (i == active - 1 || i == active + 1)
					font->FaceSize((unsigned int)(screen_w * FONT_MULT_MAZES * 1.2));
				else if (i == active) {
					glColor4f(0.10f, 0.42f, 0.035f, 1.0f);
					font->FaceSize((unsigned int)(screen_w * FONT_MULT_MAZES * 1.5));
				}
				else font->FaceSize((unsigned int)(screen_w * FONT_MULT_MAZES));
			}
			else {
				if (i == active) {
					glColor4f(0.10f, 0.42f, 0.035f, 1.0f);
					font->FaceSize((unsigned int)(screen_w * FONT_MULT * 1.5));
				}
				else font->FaceSize((unsigned int)(screen_w * FONT_MULT));
			}

			size_t pos = entryList[i].find_last_of(".");
			if (pos > 23 && pos != string::npos) {
				pos = 20;
				fontlo->Render(entryList[i].substr(0, pos).append("...").c_str());
			}
			else fontlo->Render(entryList[i].substr(0, pos).c_str());
			
			glTranslated(0, translateEntry*screen_h, 0);
		}
		
		glLoadIdentity();
		gluOrtho2D(0, screen_w, 0, screen_h);
		
		font->FaceSize((unsigned int)(screen_w * FONT_MULT));
		glTranslated(0, 0.1f*screen_h, 0);

		if (entryList.size() - 1 == active) {
			glColor4f(0.10f, 0.42f, 0.035f, 1.0f);
			font->FaceSize((unsigned int)(screen_w * FONT_MULT * 1.5));
		}
		else {
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			font->FaceSize((unsigned int)(screen_w * FONT_MULT));
		}
		
		fontlo->Render(entryList.back().c_str());

		while ((error = glGetError()) != GL_NO_ERROR)
			printf("GLERROR: %s", gluErrorString(error));

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
}

void Graphics::DrawMessage(int total, int lines, char *first, ...) {
	DrawMenuBG(true);

	char *str = first;
	va_list arglist;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screen_w, 0, screen_h);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	va_start(arglist, first);

	if (total == 1) {
		glTranslated(0, screen_h*0.55f, 0);
		fontlo->Render(str);
	}
	else if (total == 2) {
		glPushMatrix();
		glTranslated(0, screen_h*0.55f, 0);
		fontlo->Render(str);
		glPopMatrix();

		str = va_arg(arglist, char *);
		glPushMatrix();
		if (lines == 1) {
			glTranslated(0, screen_h*0.1f, 0);
		}
		else glTranslated(0, screen_h*0.5f, 0);
		fontlo->Render(str);
		glPopMatrix();
	}
	else if (total == 3) {
		glPushMatrix();
		if (lines == 1) {
			font->FaceSize((unsigned int)(screen_w * FONT_MULT * 2.5f));
			glTranslated(0, screen_h*0.5f, 0);
		}
		else glTranslated(0, screen_h*0.55f, 0);
		fontlo->Render(str);
		glPopMatrix();

		str = va_arg(arglist, char *);
		glPushMatrix();
		if (lines == 1) {
			font->FaceSize((unsigned int)(screen_w * FONT_MULT));
			glTranslated(0, screen_h*0.15f, 0);
		}
		else glTranslated(0, screen_h*0.5f, 0);
		fontlo->Render(str);
		glPopMatrix();

		str = va_arg(arglist, char *);
		glPushMatrix();
		glTranslated(0, screen_h*0.1f, 0);
		fontlo->Render(str);
		glPopMatrix();
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Graphics::DrawMaze() {
	double ratio = (double)screen_h / (double)screen_w - border_w;

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);

			glPushMatrix();
			glScaled(ratio, 1, 1);
			glTranslated(ratio, 0, 0);

			glBindTexture(GL_TEXTURE_2D, textures[BG_BOARD_TEXT]);
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2d(-1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2d(1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2d(1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex2d(-1.0f, -1.0f);
			glEnd();

			Maze::Draw();

			glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Graphics::DrawHUD() {
	double ratio = (double)screen_h / (double)screen_w + border_w;
	int t_left = (Game::TimeLeft() / 1000) + 1;

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);

			glPushMatrix();
			glTranslated(-ratio, 0, 0);
			glScaled(1-ratio, 1, 1);

			glBindTexture(GL_TEXTURE_2D, textures[BG_MENU_TEXT]);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2d(-1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2d(1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2d(1.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex2d(-1.0f, -1.0f);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, textures[LOGO_TEXT]);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2d(-0.5f, 0.8f);
				glTexCoord2f(1.0f, 1.0f); glVertex2d(0.5f, 0.8f);
				glTexCoord2f(1.0f, 0.0f); glVertex2d(0.5f, 0.6f);
				glTexCoord2f(0.0f, 0.0f); glVertex2d(-0.5f, 0.6f);
			glEnd();

		glDisable(GL_TEXTURE_2D);

		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex2d(-0.8f, -0.4f);
			glVertex2d(0.8f, -0.4f);
			glVertex2d(0.8f, -0.5f);
			glVertex2d(-0.8f, -0.5f);
			glVertex2d(-0.8f, -0.4f);
		glEnd();

		double bar = ((double)Game::TimeLeft() * 1.58f) / (double)Maze::Time() - 0.79f;

		glBegin(GL_QUADS);
			glVertex2d(-0.79f, -0.41f);
			glVertex2d(bar, -0.41f);
			glVertex2d(bar, -0.49f);
			glVertex2d(-0.79f, -0.49f);
		glEnd();

		glPopMatrix();

		glPushMatrix();
		glTranslated(-ratio, 0, 0);
		gluOrtho2D(0, screen_w, 0, screen_h);

		font->FaceSize((unsigned int)(screen_w * FONT_MULT));

		glPushMatrix();
		glTranslated(0, 0.65*screen_h, 0);
		fontlo->Render("LEVEL:");
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(0, 0.60*screen_h, 0);
		fontlo->Render(Maze::Data().Name().c_str());
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(0, 0.45*screen_h, 0);
		fontlo->Render("TIME LEFT:");
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0.40*screen_h, 0);
		char str_time[8];
		sprintf_s(str_time, "%dm %ds", t_left / 60, t_left % 60);
		fontlo->Render(str_time);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0.05*screen_h, 0);
		char str_fps[15];
		sprintf_s(str_fps, "FPS: %4.2f", fps);
		fontlo->Render(str_fps);
		glPopMatrix();

		glPopMatrix();
	glDisable(GL_BLEND);
}

void Graphics::DrawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawMaze();

	DrawHUD();
}

void Graphics::End() {
	glFinish();
}