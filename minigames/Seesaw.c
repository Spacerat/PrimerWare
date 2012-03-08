/******************************************************************************
*
* File Name          :  Seesaw.c
* Description        :  "See the saw, SEESAW! <boom>" SP/CO/VS.
*
*******************************************************************************/

// NOT FINISHED <TROLLFACE>

#define SEESAW_TIMER_GAME 1
#define SEESAW_TIMER_INSTRUCTIONS 2
#define SEESAW_X 60
#define SEESAW_Y 60
#define SEESAW_SIDE 10 // Multiple for side calcs for fulcrum drawing

#include "../Net.h"
#include "../GameHandler.h"
//#include "BalloonInflate.h"
#include "circle_api.h"
#include "../Timer.h"
#include "../Touchscreen.h"
#include <math.h>

static bool initialised = 0; // Have we run the init yet?
static u8 taps; // No. of taps.
static bool seesawDrawn = 0; // Has the balloon been drawn?
static bool rightSide = 0; // Is the right side down?

void init(struct GameData * data);
void draw(struct GameData * data);
void end(void);

	
#define SEESAWPACKET_Clicked 0
#define SEESAWPACKET_NewSize 255

#define SEESAWPACKET_TAPS_NEEDED_COOP 29
#define SEESAWPACKET_TAPS_NEEDED_SP 14
void sendPacket(bool isHost) {
	if (isHost) {
		u8 buff[2];
		buff[0] = taps;
		buff[1] = '\0';
		NET_TransmitStringPacket(PACKET_gameData, buff);
	}
	else {
		NET_TransmitStringPacket(PACKET_gameData, "");
	}
}

inline bool TapsNeeded(u8 mode) {
	return (mode == Game_SinglePlayer ?
		SEESAW_TAPS_NEEDED_SP :
		SEESAW_TAPS_NEEDED_COOP);
}


void Seesaw_run(struct GameData * data) {
	if (!initialised) init(data);
		
	//Check if the game is no longer supposed to be running, and run de-initialisation code if that's the case.
	if (!(data->code) == gameStatus_InProgress) {
		BalloonInflate_end();
		return;
	}
	// Check if we've displayed the instructions for long enough.	
	if (!TIMER_checkTimer(BALLOONINFLATE_TIMER_INSTRUCTIONS))
		return;
	
	if (!BalloonInflate_balloonDrawn) {
		BalloonInflate_balloonDrawn = 1;
		DRAW_Clear();
		BalloonInflate_draw(data);
	}
	
	// Check if we've failed.
	if (TIMER_checkTimer(BALLOONINFLATE_TIMER_GAME)) {
		BalloonInflate_end();
		data->code = gameStatus_Fail;
	}

	//In CoOp mode, check for incoming packets and deal with them.
	if (NET_GetFlags() & NETTICK_FLAG_RX && NET_GetPacketType() == PACKET_gameData && data->mode == Game_CoOp) {
		if (data->isHost) {
			BalloonInflate_taps++;
			BalloonInflate_sendPacket(data->isHost);
			BalloonInflate_draw(data);
		}
		else {
			u8 buff[2];
			NET_GetPacketData(buff);
			BalloonInflate_taps = buff[0];
			BalloonInflate_draw(data);
		}
	}
	
	// Get touch event.
	struct TouchEvent t = TOUCH_clickEvent();
	
	if (t.type == TouchType_Depressed) {
		u8 xPos = (u8)(t.position);
		u8 yPos = (u8)(t.position>>8);
	
		
		
		int xDelta = BALLOONINFLATE_X - xPos;
		int yDelta = BALLOONINFLATE_Y - yPos;
		
		// Are we inside the balloon?
		if (sqrt(pow(xDelta, 2) + pow(yDelta, 2)) <=
			BALLOONINFLATE_RADIUSMULTIPLIER * BalloonInflate_taps) {
				BalloonInflate_taps++;
				BalloonInflate_draw(data);
				if (data->mode == Game_CoOp) BalloonInflate_sendPacket(data->isHost);
		}
		
	}
	// Is the balloon fully inflated?
	if (data->isHost && BalloonInflate_taps > TapsNeeded(data->mode)) {
		BalloonInflate_end();
		data->code = gameStatus_Success;
		data->score = TIMER_ticksLeft(BALLOONINFLATE_TIMER_GAME);
	//} else {
		// data->code = gameStatus_InProgress; // NO! The game will never exit.
	}

}

void init(struct GameData * data) {
	// Set up stuff as its the first run.
	data->code = gameStatus_InProgress;
	DRAW_Clear();
	taps = 0;
	seesawDrawn = 0;
	TIMER_initTimer(SEESAW_TIMER_GAME, TIME_SECOND * 5);
	TIMER_initTimer(SEESAW_TIMER_INSTRUCTIONS, TIME_SECOND);
	initialised = 1;
	DRAW_DisplayStringWithMode(0,
							   60,
							   "See and saw",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   45,
							   "the seesaw!",
							   ALL_SCREEN, 0, 1);
}

void draw(struct GameData * data) {
	DRAW_Clear();
	
	// Draw the fulcrum.
	DRAW_Line(SEESAW_X,
			  SEESAW_Y,
			  SEESAW_X + (SEESAW_SIDE),
			  SEESAW_Y - (SEESAW_SIDE * 1.732),
			  RGB_BROWN);
	DRAW_Line(SEESAW_X + (SEESAW_SIDE),
			  SEESAW_Y - (SEESAW_SIDE * 1.732),
			  SEESAW_X - (SEESAW_SIDE),
			  SEESAW_Y - (SEESAW_SIDE * 1.732),
			  RGB_BROWN);
	DRAW_Line(SEESAW_X - (SEESAW_SIDE),
			  SEESAW_Y - (SEESAW_SIDE * 1.732),
			  SEESAW_X,
			  SEESAW_Y,
			  RGB_BROWN);
	
	if (taps % 2) {
		// Draw positive seesaw.
		DRAW_Line(SEESAW_X - (SEESAW_SIDE * 4),
				  SEESAW_Y - (SEESAW_SIDE * 1.732),
				  SEESAW_X + (SEESAW_SIDE * 4),
				  SEESAW_Y + (SEESAW_SIDE * 1.732),
				  RGB_BROWN);
		
		// Draw right arrow.
		
	} else {
		
	}
}

void end(void) {
	initialised = 0;
}