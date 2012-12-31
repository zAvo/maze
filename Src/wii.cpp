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

wiimote *Wii::wiimote_ptr;
struct wii_board_t* Wii::wii_board;
bool Wii::balanceBoard;


Wii::Wii() {
}

Wii::~Wii() {
	wiiuse_cleanup(&wiimote_ptr, 1);
}

float Wii::GetTiltX() { return balanceBoard ? wii_board->pos.x : -wiimote_ptr->orient.pitch/180; }
float Wii::GetTiltY() { return balanceBoard ? wii_board->pos.y : wiimote_ptr->orient.roll/180; }
float Wii::GetWeight() { return wii_board->weight; }

enum Status Wii::Init() {
	int i = 0;
	wiimote_ptr = *wiiuse_init(1);
	balanceBoard = false;

	if (wiiuse_find(&wiimote_ptr, 1, 2) != 1)
		return WiiERR;

	if (wiiuse_connect(&wiimote_ptr, 1) != 1)
		return WiiERR;

	HandleLeds();

	wiiuse_motion_sensing(wiimote_ptr, true);

	wiiuse_set_orient_threshold(wiimote_ptr, .5f);
	wiiuse_set_smooth_alpha(wiimote_ptr, 1.0f);
	wiiuse_set_accel_threshold(wiimote_ptr, 5000); 

	return WiiMoteOK;
}

enum Status Wii::Poll(enum Status actual_state) {
	if (wiiuse_poll(&wiimote_ptr, 1)) {
		switch (wiimote_ptr->event) {
			case WIIUSE_EVENT :
				/* a generic event occured */
				HandleEvent();
				return actual_state;

			case WIIUSE_STATUS:
				/* a status event occured */
				HandleLeds();
				return actual_state;

			case WIIUSE_DISCONNECT:
			case WIIUSE_UNEXPECTED_DISCONNECT:
				/* the wiimote disconnected */
//				handle_disconnect(wiimotes[i]);
				return WiiERR;
			
			case WIIUSE_WII_BOARD_CTRL_INSERTED:
				BoardInit();
				return WiiBoardOK;
				/* some expansion was inserted */
				break;

			case WIIUSE_WII_BOARD_CTRL_REMOVED:
				/* some expansion was removed */
				HandleLeds();
				printf("An expansion was removed.\n");
				return WiiERR;

			default:
				return actual_state;
		}
	}

	return actual_state;
}

void Wii::HandleEvent() {
	/* if a button is pressed, report it */
	if (IS_PRESSED(wiimote_ptr, WIIMOTE_BUTTON_A)) {
		if (balanceBoard)
			wiiuse_set_wii_board_calib(wiimote_ptr);
	}
}

void Wii::HandleLeds() {
	if (balanceBoard) 
		wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1);
	else {
		byte leds = (byte)(wiimote_ptr->battery_level * 4) + 1;

		switch (leds) {
			case 4:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
				break;
			case 3:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3);
				break;
			case 2:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1 | WIIMOTE_LED_2);
				break;
			case 1:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1);
				break;
			case 0:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1 | WIIMOTE_LED_4);
				break;
			default:
				wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_2 | WIIMOTE_LED_3);
		}
	}
}

void Wii::BoardInit() {
	wii_board = (wii_board_t*)&wiimote_ptr->exp.wb;
	balanceBoard = true;
	wiiuse_motion_sensing(wiimote_ptr, false);
	wiiuse_set_leds(wiimote_ptr, WIIMOTE_LED_1);
}
