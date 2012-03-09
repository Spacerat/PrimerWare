/******************************************************************************
*
* File Name          :  SquishTheBugs.c
* Description        :  Squish all the bugs.
*
*******************************************************************************/

#include <malloc.h>
#include "../Net.h"
#include "../GameHandler.h"
#include "../Touchscreen.h"
#include "../Timer.h"
#include "circle_api.h"

#define STB_TIMER_GAME 1
#define STB_TIMER_INSTRUCTIONS 2
#define STB_TIMER_DRAWING 3
#define STB_BUGS_MP 6 // Number of bugs in multiplayer
#define STB_BUGS_SP 5 // Number of bugs in singpleplayer
#define STB_BUG_SIZE 8 // Radius of bugs.


static bool numBugs = 0;
static bool initialised = 0;
static float * bugsX; // X coordinates of bugs.
static float * bugsY; // Y coordinates of bugs.
static float * bugsDir; //Bug directions
static bool * bugAlive; // Is the bug dead?
static int bugsLeft; // How many bugs remain?

static void init(struct GameData * data);
static void end(struct GameData * data);
static void draw(void);

void SquishTheBugs_run(struct GameData * data) {
	if (!initialised) init(data);
	
	// Check if the game is no longer supposed to be running, and run de-initialisation code if that's the case.
	if (data->code != gameStatus_InProgress) {
		end(data);
		return;
	}
	
	// Have we displayed the instructions for long enough?
	if (!TIMER_checkTimer(STB_TIMER_INSTRUCTIONS)) {
		return;
	}
		
	// Has a bug been killed remotely?
	if (NET_GetFlags() & NETTICK_FLAG_RX && NET_GetPacketType() == PACKET_gameData && data->mode == Game_CoOp) {
		u8 buff[2];
		NET_GetPacketData(buff);
		int bugToKill = buff[0] - 1;
		bugsLeft --; //?
		bugAlive[bugToKill] = 0;
	}
	
	// Draw the living bugs.
	if (!TIMER_isEnabled(STB_TIMER_DRAWING) || TIMER_checkTimer(STB_TIMER_DRAWING )) draw();

	// Get touch event.
	struct TouchEvent t = TOUCH_clickEvent();
	
	if (t.type == TouchType_Pressed) {
		u8 xPos = (u8)(t.position);
		u8 yPos = (u8)(t.position>>8);
		
		// Have we squashed any bugs?
		int i;
		DRAW_Circle(xPos,
					yPos,
					12,
					RGB_RED,RGB_RED, 1, 1);
		for (i = 0; i < numBugs; i++) {
			if (bugAlive[i]) {
				float xDelta = bugsX[i] - xPos;
				float yDelta = bugsY[i] - yPos;
				
				if ((xDelta * xDelta) + (yDelta * yDelta) <= (STB_BUG_SIZE * STB_BUG_SIZE) + 3) {
					// Draw a red circle as it has been squished.
					DRAW_Circle(bugsX[i],
								bugsY[i],
								STB_BUG_SIZE,
								RGB_RED,RGB_RED, 1, 1);
					
					// Kill the bug.
					bugAlive[i] = 0;
					
					bugsLeft--;
					
					// If we're multiplayer, tell the other device that i is dead.
					if (data->mode == Game_CoOp) {
						u8 buff[2];
						buff[0] = i + 1;
						buff[1] = '\0';
						NET_TransmitStringPacket(PACKET_gameData, buff);
					}
				}
			}
		}		
	}
	
	// Update the bugs positions.
	int i;
	
	for (i = 0; i < numBugs; i++) {
		
		if (bugAlive[i]) {
			//bugsDir[i] += ((rand_cmwc() % 10) / 100.0) - 0.05;
			//bugsX[i] += 1.0 * cos(bugsDir[i]);
			//bugsY[i] += 1.0 * sin(bugsDir[i]);
		
			// Can move left or right...
			//if (rand_bool()) {
				bugsX[i] = (bugsX[i] + ((rand_cmwc() % 20) / 10));
			//} else {
				bugsX[i] = (bugsX[i] - ((rand_cmwc() % 20) / 10));
			//}
			
			// ... or up or down.
			//if (rand_bool()) {
				bugsY[i] = (bugsY[i] + ((rand_cmwc() % 20) / 10));
			//} else {
				bugsY[i] = (bugsY[i] - ((rand_cmwc() % 20) / 10));
			//}
			
			// Let the screen wrap around.
			if (bugsX[i] > SCREEN_WIDTH) bugsX[i] = bugsX[i] - SCREEN_WIDTH;
			else if (bugsX[i] < 0) bugsX[i] = bugsX[i] + SCREEN_WIDTH;
				
			if (bugsY[i] > SCREEN_HEIGHT) bugsY[i] = bugsY[i] - SCREEN_HEIGHT;
			else if (bugsY[i] < 0) bugsY[i] = bugsY[i] + SCREEN_HEIGHT;
		}
	}
	
	// Have we won or lost?
	//if (data->isHost == TRUE) {
		if (bugsLeft == 0) {
			data->code = gameStatus_Success;
			data->score = TIMER_ticksLeft(STB_TIMER_GAME);
			if (data->mode != Game_CoOp || data->isHost) end(data);
		} else if (TIMER_checkTimer(STB_TIMER_GAME)) {
			data->code = gameStatus_Fail;
			if (data->mode != Game_CoOp || data->isHost) end(data);
		}
	//}
}

static void init(struct GameData * data) {
	
	// Initialise the bugs.
	int i;

	if (data->mode == Game_SinglePlayer)
		numBugs = STB_BUGS_SP;
	else
		numBugs = STB_BUGS_MP;
	
	bugsX = (float * )malloc(sizeof(float) * numBugs);
	bugsY = malloc(sizeof(float) * numBugs);
	bugsDir = malloc(sizeof(float) * numBugs);
	bugAlive = malloc(sizeof(float) * numBugs);
	

	bugsX[0] = (rand_cmwc() % 100) + 10;
	bugsY[0] = (rand_cmwc() % 100) + 10;
	bugAlive[0] = 1;
	
	for (i = 1; i < numBugs; i++) {
		bugsX[i] = bugsX[i - 1] + (rand_cmwc() % 100) + 10;
		bugsY[i] = bugsY[i - 1] + (rand_cmwc() % 100) + 10;
		bugsDir[i] = (rand_cmwc() % 360) / 360.0;
		bugAlive[i] = 1;
		
		// Let the screen wrap around.
		if (bugsX[i] > SCREEN_WIDTH) bugsX[i] = bugsX[i] - SCREEN_WIDTH;
		else if (bugsX[i] < 0) bugsX[i] = bugsX[i] + SCREEN_WIDTH;
			
		if (bugsY[i] > SCREEN_HEIGHT) bugsY[i] = bugsY[i] - SCREEN_HEIGHT;
		else if (bugsY[i] < 0) bugsY[i] = bugsY[i] + SCREEN_HEIGHT;
	}
	
	bugsLeft = numBugs;
	
	initialised = 1;
	
	// Set game status to "in progress".
	data->code = gameStatus_InProgress;
	
	// Set up drawing.
	DRAW_SetTextColor(RGB_BLUE);
	DRAW_SetBGndColor(RGB_WHITE);
	
	// Draw instructions.
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   68,
							   "Squish the",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   53,
							   "bugs!",
							   ALL_SCREEN, 0, 1);
							   
	// Initialise timers.
	TIMER_initTimer(STB_TIMER_GAME, TIME_SECOND * 6);
	TIMER_initTimer(STB_TIMER_INSTRUCTIONS, TIME_SECOND * 2);
	TIMER_disableTimer(STB_TIMER_DRAWING); // This one is disabled for initial drawing.
}

static void draw(void) {
	DRAW_Clear();
	TIMER_drawTicker(STB_TIMER_GAME);
	// Reinitialise the timer.
	TIMER_initTimer(STB_TIMER_DRAWING, 5);
	
	// Draw the bugs.
	int i;
	
	for (i = 0; i < numBugs; i++) {
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
	free(bugsX);
	free(bugsY);
	free(bugsDir);
	free(bugAlive);
	TIMER_disableTimer(STB_TIMER_INSTRUCTIONS);
}