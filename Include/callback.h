#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "header.h"

using namespace std;

class Callback {
private:	
	static int frame;
	static int time;
	static int timebase;
	static short game_status;

public:
//	Callback ();
//	~Callback ();

	static void RenderScene ();
	static void Idle ();
	static void ProcessNormalKeys ( unsigned char, int, int );
	static void PressKey ( int, int, int );
	static void SetGameStatus ( short status );
	static short GetGameStatus ();
};

#endif /* _CALLBACK_H */