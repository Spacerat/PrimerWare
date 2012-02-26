
#define TIMER_TAPPY 1


#include "tappy.h"
#include "circle_api.h"

#include "../Timer.h"
#include "../Touchscreen.h"
static int tappy_running;
static int tappy_taps;


static void minigame_tappy_draw(struct GameData * data) {
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Pump up the balloon!",
							   ALL_SCREEN, 0, 1);
							   
	char countString[8];
	UTIL_int2str(countString, tappy_taps, 8, 1);
	
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
}

static void tappy_begin(struct GameData * data) {
	//First run
	tappy_taps = 0;
	TIMER_initTimer(TIMER_TAPPY, TIME_SECOND * 5);
	tappy_running = 1;
}

static void tappy_end(struct GameData * data) {
	tappy_taps = 0;
	tappy_running = 0;
}

//Game logic
static int minigame_tappy_logic(struct GameData * data) {
	if (tappy_running == 0) {
		tappy_begin(data);
	}
	
	if (tappy_taps > 5) {
		return gameStatus_Success;
	}
	
	if (TIMER_checkTimer(TIMER_TAPPY)) {
		return gameStatus_Fail;
	}
	struct TouchEvent t = TOUCH_clickEvent();
	if (t.type == TouchType_Depressed) {
		tappy_taps++;
	}
	
	return gameStatus_InProgress;
}

void minigame_tappy(struct GameData * data) {
	int state = minigame_tappy_logic(data);
	minigame_tappy_draw(data);
	
	switch (data->mode) {
		case Game_Vs:
		case Game_CoOp:
		case Game_SinglePlayer:
			switch (state) {
				case gameStatus_Fail:
					data->code = gameStatus_Fail;
				case gameStatus_Success:
					data->code = gameStatus_Success;
				case gameStatus_InProgress:
					data->code = gameStatus_InProgress;
				default:
					break;
			}
			break;
		default:
			break;
			//assert(0); //This should never happen.
	}
	if (data->code == gameStatus_Success || data->code == gameStatus_Fail) {
		tappy_end(data);
	}
}

