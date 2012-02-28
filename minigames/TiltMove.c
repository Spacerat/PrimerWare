/******************************************************************************
*
* File Name          :  TiltMove.c
* Description        :  Get a ball from A to B by tilting.
*
*******************************************************************************/

#include "../GameHandler.h"
#include "BalloonInflate.h"
#include "circle_api.h"
#include "../Timer.h"
#include "../Touchscreen.h"
#include <math.h>
#include "../RandNo.h"

#define TILTMOVE_TIMER_GAME 1
#define TILTMOVE_TIMER_INSTRUCTIONS 2
#define TILTMOVE_TIMER_DRAWING 3

static bool initialised = 0; // Have we initialised yet?
static int ballXCoord; // X coordinate of the ball.
static int ballYCoord; // Y coordinate of the ball.
static int aXCoord; // X coordinate of A point.
static int aYCoord; // Y coordinate of A point.
static int bXCoord; // X coordinate of B point.
static int bYCoord; // Y coordinate of B point.
static int prevMEMSX; // Previous MEMS X coordinate.
static int prevMEMSY; // Previous MEMS Y coordinate.
static const int radius = 5; // Radius of ball.

void init(struct GameData * data);

void TiltMove_run(struct GameData * data) {
	if (!initialised) init(data);
	
	// Check if we've displayed the instructions for long enough.
	if (!TIMER_checkTimer(TILTMOVE_TIMER_INSTRUCTIONS))
		return;
	
	// If it's time to re-draw the track then do so.
	if (!TIMER_isEnabled(TILTMOVE_TIMER_DRAWING) || TIMER_checkTimer(TILTMOVE_TIMER_DRAWING)) {
		
		DRAW_Clear();
		
		// Re-initialise timer.
		TIMER_initTimer(TILTMOVE_TIMER_DRAWING, 20);

		// Draw lines around starting point.
		DRAW_Line(aXCoord + (radius * 2),
				  aYCoord + (radius * 2),
				  aXCoord + (radius * 2),
				  aYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord + (radius * 2),
				  aYCoord - (radius * 2),
				  aXCoord - (radius * 2),
				  aYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord - (radius * 2),
				  aYCoord - (radius * 2),
				  aXCoord - (radius * 2),
				  aYCoord + (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord - (radius * 2),
				  aYCoord + (radius * 2),
				  aXCoord + (radius * 2),
				  aYCoord + (radius * 2),
				  RGB_BLUE);
				  
		// Draw lines around finishing point.
		DRAW_Line(bXCoord + (radius * 2),
				  bYCoord + (radius * 2),
				  bXCoord + (radius * 2),
				  bYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(bXCoord + (radius * 2),
				  bYCoord - (radius * 2),
				  bXCoord - (radius * 2),
				  bYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(bXCoord - (radius * 2),
				  bYCoord - (radius * 2),
				  bXCoord - (radius * 2),
				  bYCoord + (radius * 2),
				  RGB_BLUE);
		DRAW_Line(bXCoord - (radius * 2),
				  bYCoord + (radius * 2),
				  bXCoord + (radius * 2),
				  bYCoord + (radius * 2),
				  RGB_BLUE);
				  
		// Connect the two points together.
		DRAW_Line(aXCoord + (radius * 2),
				  aYCoord + (radius * 2),
				  bXCoord + (radius * 2),
				  bYCoord + (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord + (radius * 2),
				  aYCoord - (radius * 2),
				  bXCoord + (radius * 2),
				  bYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord - (radius * 2),
				  aYCoord - (radius * 2),
				  bXCoord - (radius * 2),
				  bYCoord - (radius * 2),
				  RGB_BLUE);
		DRAW_Line(aXCoord - (radius * 2),
				  aYCoord + (radius * 2),
				  bXCoord - (radius * 2),
				  bYCoord + (radius * 2),
				  RGB_BLUE);
				  
		// Draw the ball.
		DRAW_Circle(ballXCoord,
					ballYCoord,
					radius,
					RGB_BLUE,RGB_BLUE, 1, 1);
	}
	
	// Get current MEMS coordinates and calculate delta.
	int curMEMSX = (MEMS_GetInfo())->OutX_F64;
	int curMEMSY = (MEMS_GetInfo())->OutY_F64;
	
	int deltaMEMSX = curMEMSX - prevMEMSX;
	int deltaMEMSY = curMEMSY - prevMEMSY;
	
	//prevMEMSX = curMEMSX;
	//prevMEMSY = curMEMSY;
	
	// Update ball position.
	ballXCoord += (int)((float) deltaMEMSX * 0.0003);
	ballYCoord += (int)((float) deltaMEMSY * 0.0003);
}

void init(struct GameData * data) {
	// Initialise random number generator. (do we want to do this?)
	init_rand(35);
	
	// Set start and finish to random locations.
	aXCoord = (rand_cmwc() % 100) + 10;
	aYCoord = (rand_cmwc() % 100) + 10;
	bXCoord = (rand_cmwc() % 100) + 10;
	bYCoord = (rand_cmwc() % 100) + 10;
	
	// If A and B aren't different enough, re-init next time.
	if (aXCoord - bXCoord < 30 || aYCoord - bYCoord < 30) {
		return;
	}
		
	initialised = 1;
	
	// Set game status to "in progress".
	data->code = gameStatus_InProgress;
	
	// Set up drawing.
	DRAW_SetTextColor(RGB_BLUE);
	DRAW_SetBGndColor(RGB_WHITE);
	
	
	// Set ball initial position to A.
	ballXCoord = aXCoord;
	ballYCoord = aYCoord;
	
	// Get initial MEMS position.
	prevMEMSX = (MEMS_GetInfo())->OutX_F4;
	prevMEMSY = (MEMS_GetInfo())->OutY_F4;
	
	// Draw instructions.
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   75,
							   "Get the ball",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   60,
							   "to the other",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   45,
							   "side!",
							   ALL_SCREEN, 0, 1);
							   
	// Initialise timers.
	TIMER_initTimer(TILTMOVE_TIMER_GAME, TIME_SECOND * 5);
	TIMER_initTimer(TILTMOVE_TIMER_INSTRUCTIONS, TIME_SECOND);
	TIMER_disableTimer(TILTMOVE_TIMER_DRAWING); // This one is disabled for initial drawing.
}