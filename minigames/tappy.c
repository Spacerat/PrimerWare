
#define TIMER_TAPPY 1

#define TAPPY_PLAYING

static tappy_running;
static tappy_taps;

static struct GameStatus status;


static void minigame_tappy_draw() {
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

static int minigame_tappy_logic() {
	if (tappy_running == 0) {
		//First run
		tappy_taps = 0;
		TIMER_initTimer(TIMER_TAPPY, TIME_SECOND * 5);
		tappy_running = 1;
		status.code = gameStatus_InProgress;
	}
	
	if (tappy_taps > 50) {
		return 1;
	}
	
	if (TOUCHSCR_IsPressed()) {
		tappy_tapps++;
	}
}

struct GameStatus minigame_tappy() {
	
	// END TIMING CODE!
	
	switch (minigame_tappy_logic()) {
		case TAPPY_FAIL:
			status.code = gameStatus_Fail;
		case TAPPY_WON:
			status.code = gameStatus_Success;
		case TAPPY_PLAYING:
		default:
			break;
	}
		
	return status;
}

