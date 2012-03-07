/******************************************************************************
*
* File Name          :  SquishTheBugs.c
* Description        :  Squish all the bugs.
*
*******************************************************************************/


#include "../Net.h"
#include "../GameHandler.h"
#include "../Touchscreen.h"
#include "../Timer.h"
#include "circle_api.h"

#define STB_TIMER_GAME 1
#define STB_TIMER_INSTRUCTIONS 2
#define STB_TIMER_DRAWING 3
#define STB_BUGS 5 // Number of bugs.
#define STB_BUG_SIZE 2 // Radius of bugs.

static bool initialised = 0;
static float bugsX[STB_BUGS]; // X coordinates of bugs.
static float bugsY[STB_BUGS]; // Y coordinates of bugs.
static bool bugAlive[STB_BUGS]; // Is the bug dead?
static int bugsLeft; // How many bugs remain?

static void init(struct GameData * data);
static void end(struct GameData * data);
static void draw(void);

void SquishTheBugs_run(struct GameData * data) {
	if (!initialised) init(data);
		
	// Have we displayed the instructions for long enough?
	if (!TIMER_checkTimer(STB_TIMER_INSTRUCTIONS)) {
		return;
	}
		
	// Draw the living bugs.
	if (!TIMER_isEnabled(STB_TIMER_DRAWING) || TIMER_checkTimer(STB_TIMER_DRAWING )) draw();

	// Get touch event.
	struct TouchEvent t = TOUCH_clickEvent();
	
	if (t.type == TouchType_Depressed) {
		u8 xPos = (u8)(t.position);
		u8 yPos = (u8)(t.position>>8);
		
		// Have we squashed any bugs?
		int i;
		
		for (i = 0; i < STB_BUGS; i++) {
			if (bugAlive[i]) {
				int xDelta = bugsX[i] - xPos;
				int yDelta = bugsY[i] - yPos;
				
				if ((xDelta * xDelta) + (yDelta * yDelta) <= STB_BUG_SIZE * STB_BUG_SIZE * 1.2) {
					// Draw a red circle as it has been squished.
					DRAW_Circle(bugsX[i],
								bugsY[i],
								STB_BUG_SIZE,
								RGB_RED,RGB_RED, 1, 1);
					
					// Kill the bug.
					bugAlive[i] = 0;
				}
			}
		}		
	}
	
	// Update the bugs positions.
	int i;
	
	for (i = 0; i < STB_BUGS; i++) {
		
		if (bugAlive[i]) {
			// Can move left or right...
			if (rand_bool()) {
				bugsX[i] = (bugsX[i] + ((rand_cmwc() % 10) / 10));
			} else {
				bugsX[i] = (bugsX[i] - ((rand_cmwc() % 10) / 10));
			}
			
			// ... or up or down.
			if (rand_bool()) {
				bugsY[i] = (bugsY[i] + ((rand_cmwc() % 10) / 10));
			} else {
				bugsY[i] = (bugsY[i] - ((rand_cmwc() % 10) / 10));
			}
			
			// Let the screen wrap around.
			if (bugsX[i] > SCREEN_WIDTH) bugsX[i] = bugsX[i] - SCREEN_WIDTH;
			else if (bugsX[i] < 0) bugsX[i] = bugsX[i] + SCREEN_WIDTH;
				
			if (bugsY[i] > SCREEN_HEIGHT) bugsY[i] = bugsY[i] - SCREEN_HEIGHT;
			else if (bugsY[i] < 0) bugsY[i] = bugsY[i] + SCREEN_HEIGHT;
		}
	}
	
	// Have we won or lost?
	if (bugsLeft == 0) {
		data->code = gameStatus_Success;
		data->score = TIMER_ticksLeft(STB_TIMER_GAME);
		end(data);
	} else if (TIMER_checkTimer(STB_TIMER_GAME)) {
		data->code = gameStatus_Fail;
		end(data);
	}
	
}

static void init(struct GameData * data) {
	
	// Initialise the bugs.
	int i;
	
	for (i = 0; i < STB_BUGS; i++) {
		bugsX[i] = (rand_cmwc() % 100) + 10;
		bugsY[i] = (rand_cmwc() % 100) + 10;
		bugAlive[i] = 1;
	}
	
	bugsLeft = STB_BUGS;
	
	initialised = 1;
	
	// Set game status to "in progress".
	data->code = gameStatus_InProgress;
	
	// Set up drawing.
	DRAW_SetTextColor(RGB_BLUE);
	DRAW_SetBGndColor(RGB_WHITE);
	
	// Draw instructions.
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   75,
							   "Squish the",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   60,
							   "bugs!",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   45,
							   "side!",
							   ALL_SCREEN, 0, 1);
							   
	// Initialise timers.
	TIMER_initTimer(STB_TIMER_GAME, TIME_SECOND * 6);
	TIMER_initTimer(STB_TIMER_INSTRUCTIONS, TIME_SECOND * 2);
	TIMER_disableTimer(STB_TIMER_DRAWING); // This one is disabled for initial drawing.

}

static void draw(void) {
	DRAW_Clear();
	
	// Reinitialise the timer.
	TIMER_initTimer(STB_TIMER_DRAWING, 20);
	
	// Draw the bugs.
	int i;
	
	for (i = 0; i < STB_BUGS; i++) {
		if (bugAlive[i]) {
			DRAW_Circle(bugsX[i],
						bugsY[i],
						STB_BUG_SIZE,
						RGB_GREEN,RGB_GREEN, 1, 1);
		}
	}
}

static void end(struct GameData * data) {
	initialised = 0;
}