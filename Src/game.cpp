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

enum Status Game::status;
int Game::t_total;
int Game::t_start;
bool Game::pause;


enum Status Game::Init(int *argc, char *argv[]) {
	status = WiiNo;
	pause = false;

	if (Graphics::Init(argc, argv) != 0) {
		status = ERR;
		return ERR;
	}

	status = Menu::Init();
	if (status == LevelNoLevels)
		return LevelNoLevels;
	else if (status != MenuOK) {
		status = MenuERR;
		return MenuERR;
	}

	status = WiiNo;
	return status;
}

void Game::Idle() {
	Graphics::SetFPS();
	switch(status) {
		case WiiNo:
			status = Wii::Init();
			t_start = Graphics::Time();
		case WiiERR:
		case LevelNoLevels:
			break;
		case GameOK:
			status = Wii::Poll(status);
			if (status == GameOK)
				status = Physics::ComputePosition();
			if (status == GameOK)
				if (TimeLeft() < 0)
					status = LevelTimeUp;
			break;
		case Quit:
			Graphics::ExitMainLoop();
			break;
		default:
			status = Wii::Poll(status);
			break;
	}

	Graphics::PostRedisplay();
}

void Game::DrawScene() {
	switch(status) {
		case WiiNo:
			Graphics::DrawMessage(1, 0, "Checking for WiiMote and Balance Board");
			// Checking for WiiMote and Balance Board
			break;
		case WiiMoteOK:
			Graphics::DrawMessage(1, 0, "Checking for WiiMote and Balance Board");
			if (Graphics::Time() - t_start > 3000)
				status = WiiBoardNo;
			// Checking for WiiMote and Balance Board
			break;
		case WiiBoardNo:
			Graphics::DrawMessage(2, 1, "Balance board not found", "Press ENTER to continue with WiiMote");
			break;
		case WiiBoardOK:
			Graphics::DrawMessage(1, 0, "Checking for WiiMote and Balance Board");
			// Checking for WiiMote and Balance Board
			status = WiiOK;
			break;
		case WiiERR:
			Graphics::DrawMessage(3, 2, "An error occurred with WiiMote", "and/or Balance Board", "Press any key to exit");
			break;
			// An error occurred with WiiMote and/or Balance Board
		case WiiOK:
			Graphics::DrawMessage(1, 0, "WiiMote and Balance Board found and ready");
			// WiiMote and Balance Board found and ready
			status = MenuIn;
			break;
		case MenuIn:
			// Draw Menu
			Menu::Draw();
			break;
		case GameOK:
			Graphics::DrawScene();
			// Draw HUD
			// Draw Maze BG
			// Draw Maze
			// Draw Ball
			// Maze.Draw();
			break;
		case GameERR:
			// An error occurred during the game
		case LevelComplete:
			PauseGame();
			Graphics::DrawScene();
			Graphics::DrawMessage(3, 1, "LEVEL COMPLETE!", "Press ENTER to start next level", "Press ESC to return to menu");
			break;
			// Level Complete! Menu
		case LevelTimeUp:
			PauseGame();
			Graphics::DrawScene();
			Graphics::DrawMessage(3, 1, "TIME IS UP!", "Press ENTER to retry level", "Press ESC to return to menu");
			break;
			// Time is Up! Menu
		case LevelLost:
			PauseGame();
			Graphics::DrawScene();
			Graphics::DrawMessage(3, 1, "GAME OVER!", "Press ENTER to retry level", "Press ESC to return to menu");
			break;
			// Level Lost! Menu
		case LevelNoLevels:
			Graphics::DrawMessage(2, 1, "No (valid) mazes found", "Press any key to exit");
			break;
		case Quit:
			Graphics::ExitMainLoop();
		default:
			break;
	}

	Graphics::SwapBuffers();
}

void Game::ProcessNormalKeys(unsigned char key, int x, int y) {
	enum Status ret;

	switch(key) {
		// User pressed Enter key
		case Graphics::Enter:
			switch(status) {
				case MenuIn:
					ret = Menu::Do();
					if (ret == GameLoad) {
						status = StartLevel(MenuLevels::GetActiveLevel());
					}
					else if (ret == GameOK) {
						ContinueGame();
						status = GameOK;
					}
					else if (ret == Quit) {
						status = Quit;
					}
					break;
				case LevelComplete:
					status = StartLevel(MenuLevels::GetNextLevel());
					break;
				case LevelTimeUp:
					status = StartLevel(MenuLevels::GetActiveLevel());
					break;
				case LevelLost:
					status = StartLevel(MenuLevels::GetActiveLevel());
					break;
				case WiiBoardNo:
					status = WiiOK;
					break;
				case WiiERR:
				case GameERR:
				case LevelNoLevels:
				case ERR:
					status = Quit;
					break;
			}
			break;
		// User pressed Escape key
		case Graphics::Esc:
			switch(status) {
				case MenuIn:
					ret = Menu::Escape();
					if (ret == Quit) {
						status = Quit;
						//Graphics::ExitMainLoop();
					}
					else if (ret == GameOK) {
						ContinueGame();
						status = GameOK;
					}
					break;
				case GameOK:
					PauseGame();
					status = MenuIn;
					break;
				case LevelComplete:
				case LevelTimeUp:
				case LevelLost:
					Menu::SetMenuMain();
					status = MenuIn;
					break;
				case WiiERR:
				case LevelNoLevels:
				case GameERR:
				case ERR:
					status = Quit;
					break;
			}
			break;
		// User pressed another key
		default:
			switch(status) {
				case WiiERR:
				case GameERR:
				case ERR:
					status = Quit;
					break;
			}
	}
}

void Game::ProcessSpecialKeys(int key, int x, int y) {
	switch(key) {
		// User pressed Up key
		case Graphics::Up:
			if (status == MenuIn)
				Menu::Prev();
			break;
		// User pressed Down key
		case Graphics::Down:
			if (status == MenuIn)
				Menu::Next();
			break;
		// User pressed another key
		default:
			switch(status) {
				case WiiERR:
				case LevelNoLevels:
				case GameERR:
				case ERR:
					status = Quit;
					break;
			}
	}
}

enum Status Game::StartLevel(const char *level) {
	Maze::Free();
	if (Maze::Load(level) == 0) {
		Physics::Init();

		t_total = Maze::Time();
		t_start = Graphics::Time();

		pause = false;

		return GameOK;
	}
	else return GameERR;
}

void Game::ContinueGame() {
	Physics::ContinueGame();
	pause = false;
	t_start = Graphics::Time();
}

void Game::PauseGame() {
	if (!pause) {
		pause = true;
		t_total = t_total - (Graphics::Time() - t_start);
	}
}

int Game::TimeLeft() {
	if (pause)
		return t_total;
	else return t_total - Graphics::Time() + t_start;
}