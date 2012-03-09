/******************************************************************************
*
* File Name          :  BalloonInflate.c
* Description        :  Inflate the balloon by tapping. SP/CO/VS.
*
*******************************************************************************/

#define BALLOONINFLATE_TIMER_GAME 1
#define BALLOONINFLATE_TIMER_INSTRUCTIONS 2
#define BALLOONINFLATE_X 60
#define BALLOONINFLATE_Y 60
#define BALLOONINFLATE_RADIUSMULTIPLIER 50

#include "../Net.h"
#include "../GameHandler.h"
#include "BalloonInflate.h"
#include "circle_api.h"
#include "../Timer.h"
#include "../Touchscreen.h"
#include <math.h>

static bool BalloonInflate_initialised = 0; // Have we run the init yet?
static u8 BalloonInflate_taps; // No. of taps.
static bool BalloonInflate_balloonDrawn = 0; // Has the balloon been drawn?

void BalloonInflate_init(struct GameData * data);
void BalloonInflate_draw(struct GameData * data);
void BalloonInflate_end(void);

	
#define BALLOONPACKET_Clicked 0
#define BALLOONPACKET_NewSize 255

#define BALLOONINFLATE_TAPS_NEEDED_COOP 29
#define BALLOONINFLATE_TAPS_NEEDED_SP 14
void BalloonInflate_sendPacket(bool isHost) {
	if (isHost) {
		u8 buff[2];
		buff[0] = BalloonInflate_taps;
		buff[1] = '\0';
		NET_TransmitStringPacket(PACKET_gameData, buff);
	}
	else {
		NET_TransmitStringPacket(PACKET_gameData, "");
	}
}

inline bool TapsNeeded(u8 mode) {
	return (mode == Game_SinglePlayer ?
		BALLOONINFLATE_TAPS_NEEDED_SP :
		BALLOONINFLATE_TAPS_NEEDED_COOP);
}


void BalloonInflate_run(struct GameData * data) {
	if (!BalloonInflate_initialised) BalloonInflate_init(data);
		
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
	if (BalloonInflate_taps > TapsNeeded(data->mode)) {
		//BalloonInflate_end();
		data->code = gameStatus_Success;
		data->score = TIMER_ticksLeft(BALLOONINFLATE_TIMER_GAME);
	}

	TIMER_drawTicker(BALLOONINFLATE_TIMER_GAME);
}

void BalloonInflate_init(struct GameData * data) {
	// Set up stuff as its the first run.
	data->code = gameStatus_InProgress;
	DRAW_Clear();
	BalloonInflate_taps = 1; // Set to 1 to draw initial balloon.
	TIMER_initTimer(BALLOONINFLATE_TIMER_GAME, TIME_SECOND * 5);
	TIMER_initTimer(BALLOONINFLATE_TIMER_INSTRUCTIONS, TIME_SECOND);
	BalloonInflate_initialised = 1;
	DRAW_DisplayStringWithMode(0,
							   60,
							   "Inflate the",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   45,
							   "balloon!",
							   ALL_SCREEN, 0, 1);
}

void BalloonInflate_draw(struct GameData * data) {
	DRAW_Circle(BALLOONINFLATE_X,
				BALLOONINFLATE_Y,
				BALLOONINFLATE_RADIUSMULTIPLIER * (((float)BalloonInflate_taps + 1) / (float)TapsNeeded(data->mode)),
				RGB_BURGUNDY, RGB_RED, 1, 1);
}

void BalloonInflate_end(void) {
	// Reset stuff at the end.
	BalloonInflate_taps = 1;
	BalloonInflate_initialised = 0;
	BalloonInflate_balloonDrawn = 0;
}