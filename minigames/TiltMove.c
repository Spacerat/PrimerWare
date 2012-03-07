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
static float ballXCoord; // X coordinate of the ball.
static float ballYCoord; // Y coordinate of the ball.
static int aXCoord; // X coordinate of A point.
static int aYCoord; // Y coordinate of A point.
static int bXCoord; // X coordinate of B point.
static int bYCoord; // Y coordinate of B point.
static bool negativeGradient; // Is the gradient of the line from A to B negative?
static bool aAtBottom; // Is A below B?
static float xCoords[6]; // X and Y coordinates of the points that make up the track.
static float yCoords[6];
static float prevMEMSX; // Previous MEMS X coordinate.
static float prevMEMSY; // Previous MEMS Y coordinate.
static const int radius = 3; // Radius of ball.
#define radMult 3
static bool randomInit = 0; // Have we initialised the PRNG?

void init(struct GameData * data);
void end(struct GameData * data);

int sign(float x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

float absolute(float x) {
	if (x < 0) return -x;
	else return x;
}

void TiltMove_run(struct GameData * data) {
	if (!initialised) {
		init(data);
		return;
	}
	
	// Check if we've displayed the instructions for long enough.
	if (!TIMER_checkTimer(TILTMOVE_TIMER_INSTRUCTIONS)) {
		// Get initial MEMS position. This will ensure our MEMS is correct at the end of
		// displaying instructions (ugly but meh).
		prevMEMSX = (MEMS_GetInfo())->OutX_F4;
		prevMEMSY = (MEMS_GetInfo())->OutY_F4;
		
		return;
	}
	
	// Have we run out of time?
	if (TIMER_checkTimer(TILTMOVE_TIMER_GAME)) {
		end(data);
		data->code = gameStatus_Fail;
	}
	
	// If it's time to re-draw the track then do so.
	if (!TIMER_isEnabled(TILTMOVE_TIMER_DRAWING) || TIMER_checkTimer(TILTMOVE_TIMER_DRAWING)) {
		
		DRAW_Clear();
		
		// Re-initialise timer.
		TIMER_initTimer(TILTMOVE_TIMER_DRAWING, 20);
		
		// Draw the track.
		int i;
		for (i = 0; i < 6 - 1; i++) {
			DRAW_Line(xCoords[i],
					  yCoords[i],
					  xCoords[i + 1],
					  yCoords[i + 1],
					  RGB_BLUE);
		}
		
		DRAW_Line(xCoords[5],
				  yCoords[5],
				  xCoords[0],
				  yCoords[0],
				  RGB_BLUE);

		// Draw a dot at B.
		DRAW_Circle(bXCoord,
					bYCoord,
					radius,
					RGB_RED,RGB_WHITE, 1, 1);
	}
	
	// Draw the ball.
	DRAW_Circle(ballXCoord,
				ballYCoord,
				radius,
				RGB_BLUE,RGB_BLUE, 1, 1);
	
	// Get current MEMS coordinates and calculate delta.
	float curMEMSX = (MEMS_GetInfo())->OutX_F4;
	float curMEMSY = (MEMS_GetInfo())->OutY_F4;
	
	float deltaMEMSX = curMEMSX - prevMEMSX;
	float deltaMEMSY = curMEMSY - prevMEMSY;

	// Update ball position.
	ballXCoord += (deltaMEMSX / 1000);
	ballYCoord += (deltaMEMSY / 1000);
	
	// Are we still within the track?
	/*
	 * If we look at the lines going around the track clockwise,
	 * the ball must always be to its right, regardless of the
	 * gradient of the track.
	 */
	bool inside = 1; // Are we inside?
	int position = -1; // -1 when on right.

	int i;
	for (i = 0; i < 6 - 1; i++) {
		position = sign( (xCoords[i + 1] - xCoords[i])*(ballYCoord - yCoords[i])
					   - (yCoords[i + 1] - yCoords[i])*(ballXCoord - xCoords[i]) );
		inside = inside & (position == -1);
	}

	position = sign( (xCoords[0] - xCoords[5])*(ballYCoord - yCoords[5])
				   - (yCoords[0] - yCoords[5])*(ballXCoord - xCoords[5]) );
	inside = inside & (position == -1);
	
	if (!inside) {
		data->code = gameStatus_Fail;
		end(data);
	}
	
	// Are we at B yet?
	if ( absolute( ballXCoord - bXCoord) <= radius &&
		 absolute( ballYCoord - bYCoord) <= radius) {
		
		data->code = gameStatus_Success;
		data->score = TIMER_ticksLeft(TILTMOVE_TIMER_GAME);
		end(data);
	}
}

__attribute__((section(".rodata"))) void init(struct GameData * data) {
	// Initialise random number generator. (do we want to do this?)
	if (!randomInit) {
		init_rand(23);
		randomInit = 1;
	}
	
	// Set start and finish to random locations.
	aXCoord = (rand_cmwc() % 90) + 10;
	aYCoord = (rand_cmwc() % 30) + 10;
	bXCoord = (rand_cmwc() % 90) + 10;
	bYCoord = (rand_cmwc() % 30) + 10 + 70;
	
	// If A and B aren't different enough, re-init next time.
	if (aXCoord - bXCoord < (radius * 5) || aYCoord - bYCoord < (radius * 5)) {
		return;
	}
	
	// Maybe flip.
	if (rand_cmwc() % 2 == 1) {
		int temp = aXCoord;
		aXCoord = aYCoord;
		aYCoord = temp;
		
		temp = bXCoord;
		bXCoord = bYCoord;
		bYCoord = temp;
	}
		
	initialised = 1;
	
	// Find out if we have a negative gradient for the path.
	if (bXCoord - aXCoord < 0 && bYCoord - aYCoord < 0) {
		negativeGradient = 1;
	} else {
		negativeGradient = 0;
	}
	
	// Find out if A is below B.
	if (aYCoord - bYCoord < 0) {
		aAtBottom = 1;
	} else {
		aAtBottom = 0;
	}
	
	// Set up the coordinates of the track.
	if (negativeGradient && aAtBottom) {
		xCoords[0] = aXCoord - (radius * radMult); 
		xCoords[1] = bXCoord - (radius * radMult);
		xCoords[2] = bXCoord + (radius * radMult);
		xCoords[3] = bXCoord + (radius * radMult);
		xCoords[4] = aXCoord + (radius * radMult);
		xCoords[5] = aXCoord - (radius * radMult);
		yCoords[0] = aYCoord + (radius * radMult); 
		yCoords[1] = bYCoord + (radius * radMult); 
		yCoords[2] = bYCoord + (radius * radMult);
		yCoords[3] = bYCoord - (radius * radMult);
		yCoords[4] = aYCoord - (radius * radMult);
		yCoords[5] = aYCoord - (radius * radMult);
	} else if (negativeGradient && !aAtBottom) {
		xCoords[0] = bXCoord - (radius * radMult);
		xCoords[1] = aXCoord - (radius * radMult); 
		xCoords[2] = aXCoord + (radius * radMult);
		xCoords[3] = aXCoord + (radius * radMult);
		xCoords[4] = bXCoord + (radius * radMult);
		xCoords[5] = bXCoord - (radius * radMult);
		yCoords[0] = bYCoord + (radius * radMult); 
		yCoords[1] = aYCoord + (radius * radMult); 
		yCoords[2] = aYCoord + (radius * radMult);
		yCoords[3] = aYCoord - (radius * radMult);
		yCoords[4] = bYCoord - (radius * radMult);
		yCoords[5] = bYCoord - (radius * radMult);
	} else if (!negativeGradient && aAtBottom) {
		xCoords[0] = bXCoord - (radius * radMult); 
		xCoords[1] = bXCoord + (radius * radMult); 
		xCoords[2] = aXCoord + (radius * radMult);
		xCoords[3] = aXCoord + (radius * radMult);
		xCoords[4] = aXCoord - (radius * radMult);
		xCoords[5] = bXCoord - (radius * radMult);
		yCoords[0] = bYCoord + (radius * radMult); 
		yCoords[1] = bYCoord + (radius * radMult); 
		yCoords[2] = aYCoord + (radius * radMult);
		yCoords[3] = aYCoord - (radius * radMult);
		yCoords[4] = aYCoord - (radius * radMult);
		yCoords[5] = bYCoord - (radius * radMult);
	} else if (!negativeGradient && !aAtBottom) {
		xCoords[0] = aXCoord - (radius * radMult);
		xCoords[1] = aXCoord + (radius * radMult); 
		xCoords[2] = bXCoord + (radius * radMult);
		xCoords[3] = bXCoord + (radius * radMult);
		xCoords[4] = bXCoord - (radius * radMult);
		xCoords[5] = aXCoord - (radius * radMult);
		yCoords[0] = aYCoord + (radius * radMult); 
		yCoords[1] = aYCoord + (radius * radMult); 
		yCoords[2] = bYCoord + (radius * radMult);
		yCoords[3] = bYCoord - (radius * radMult);
		yCoords[4] = bYCoord - (radius * radMult);
		yCoords[5] = aYCoord - (radius * radMult);
	}
	
	// Set game status to "in progress".
	data->code = gameStatus_InProgress;
	
	// Set up drawing.
	DRAW_SetTextColor(RGB_BLUE);
	DRAW_SetBGndColor(RGB_WHITE);
	
	// Set ball initial position to A.
	ballXCoord = aXCoord;
	ballYCoord = aYCoord;
	
	// Draw instructions.
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   105,
							   "Get the ball",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   90,
							   "to the other",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   75,
							   "side!",
							   ALL_SCREEN, 0, 1);
							   
	DRAW_DisplayStringWithMode(0,
							   45,
							   "Lay me",
							   ALL_SCREEN, 0, 1);
	DRAW_DisplayStringWithMode(0,
							   30,
							   "flat",
							   ALL_SCREEN, 0, 1);

							   
	// Initialise timers.
	TIMER_initTimer(TILTMOVE_TIMER_GAME, TIME_SECOND * 6);
	TIMER_initTimer(TILTMOVE_TIMER_INSTRUCTIONS, TIME_SECOND * 2);
	TIMER_disableTimer(TILTMOVE_TIMER_DRAWING); // This one is disabled for initial drawing.
}

__attribute__((section(".rodata"))) void end(struct GameData * data) {
	initialised = 0;
	randomInit = 0;
}