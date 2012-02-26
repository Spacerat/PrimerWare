
#define TIMER_TAPPY 1


#include "tappy.h"
#include "circle_api.h"

#include "../Timer.h"
#include "../Touchscreen.h"
static int tappy_running;
static int tappy_taps;


static void minigame_tappy_draw(struct GameData * data) {
	
	

							   

}

static void tappy_begin() {
	//First run
	DRAW_Clear();
	tappy_taps = 0;
	TIMER_initTimer(TIMER_TAPPY, TIME_SECOND * 3);
	tappy_running = 1;
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Tap quickly!",
							   ALL_SCREEN, 0, 1);
}

static void tappy_end() {
	tappy_taps = 0;
	tappy_running = 0;
}

//Game logic
static int minigame_tappy_logic(struct GameData * data) {
	if (tappy_running == 0) {
		tappy_begin();
	}
	

	if (TIMER_checkTimer(TIMER_TAPPY)) {
		return gameStatus_Fail;
	}
	char nString[8];
	unsigned int ticksleft = TIMER_ticksLeft(TIMER_TAPPY);
	UTIL_int2str(nString, ticksleft, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   40,
							   nString,
							   ALL_SCREEN, 0, 1);
	struct TouchEvent t = TOUCH_clickEvent();
	if (t.type == TouchType_Depressed) {
		tappy_taps++;
		
		UTIL_int2str(nString, tappy_taps, 8, 1);
		
		DRAW_DisplayStringWithMode(0,
								   10,
								   nString,
								   ALL_SCREEN, 0, 1);
	}
	if (tappy_taps > 5) {
		return gameStatus_Success;
	}
	return gameStatus_InProgress;
}

void minigame_tappy(struct GameData * data) {
	int state = minigame_tappy_logic(data);
	
	
	switch (data->mode) {
		case Game_Vs:
		case Game_CoOp:
		case Game_SinglePlayer:
			switch (state) {
				case gameStatus_Fail:
					data->code = gameStatus_Fail;
					break;
				case gameStatus_Success:
					data->code = gameStatus_Success;
					break;
				case gameStatus_InProgress:
					data->code = gameStatus_InProgress;
					break;
				default:
					break;
			}
			break;
		default:
			break;
			//assert(0); //This should never happen.
	}
	if (data->code == gameStatus_Success || data->code == gameStatus_Fail) {
		tappy_end();
	}
}

