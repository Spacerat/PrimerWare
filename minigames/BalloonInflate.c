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
#define BALLOONINFLATE_RADIUSMULTIPLIER 10

#include "../GameHandler.h"
#include "BalloonInflate.h"
#include "circle_api.h"
#include "../Timer.h"
#include "../Touchscreen.h"
#include <math.h>

static bool BalloonInflate_initialised = 0; // Have we run the init yet?
static int BalloonInflate_taps; // No. of taps.
static bool BalloonInflate_balloonDrawn = 0; // Has the balloon been drawn?

void BalloonInflate_init(struct GameData * data);
void BalloonInflate_draw(void);
void BalloonInflate_end(void);

void BalloonInflate_run(struct GameData * data) {
	if (!BalloonInflate_initialised) BalloonInflate_init(data);
	
	// Check if we've displayed the instructions for long enough.	
	if (!TIMER_checkTimer(BALLOONINFLATE_TIMER_INSTRUCTIONS))
		return;
	
	if (!BalloonInflate_balloonDrawn) {
		BalloonInflate_balloonDrawn = 1;
		DRAW_Clear();
		BalloonInflate_draw();
	}
	
	// Check if we've failed.
	if (TIMER_checkTimer(BALLOONINFLATE_TIMER_GAME)) {
		BalloonInflate_end();
		data->code = gameStatus_Fail;
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
				BalloonInflate_draw();
		}
		
		// Is the balloon fully inflated?
		if (BalloonInflate_taps > 5) {
			BalloonInflate_end();
			data->code = gameStatus_Success;
			data->score = TIMER_ticksLeft(BALLOONINFLATE_TIMER_GAME);
		} else {
			data->code = gameStatus_InProgress;
		}
	}
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

void BalloonInflate_draw(void) {
	DRAW_Circle(BALLOONINFLATE_X,
				BALLOONINFLATE_Y,
				BALLOONINFLATE_RADIUSMULTIPLIER * BalloonInflate_taps,
				RGB_BURGUNDY, RGB_RED, 1, 1);
}

void BalloonInflate_end(void) {
	// Reset stuff at the end.
	BalloonInflate_taps = 1;
	BalloonInflate_initialised = 0;
	BalloonInflate_balloonDrawn = 0;
}