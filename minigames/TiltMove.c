/******************************************************************************
*
* File Name          :  TiltMove.c
* Description        :  Get a ball from A to B by tilting.
*
*******************************************************************************/

#include "../Net.h"
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

static enum Tiltmove_State {
	TM_State_START,
	TM_State_INSTRUCTIONS,
	TM_State_PLAYING
} state;


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

static void init(struct GameData * data);
static void end(struct GameData * data);
static void beginPlay(struct GameData * data);
static void play(struct GameData * data);
	
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
	
	//Check if the game is no longer supposed to be running, and run de-initialisation code if that's the case.
	if (data->code != gameStatus_InProgress) {
		end(data);
		return;
	}

	switch (state) {
		case TM_State_START:
			init(data);
			break;
		case TM_State_INSTRUCTIONS:
			if (data->isHost == FALSE && NET_GetFlags() & NETTICK_FLAG_RX && NET_GetPacketType() == 42) {
				u8 buff[sizeof(int) * 4 + 1];
				int * ibuff = (int*) buff;
				NET_GetPacketData(buff);
				aXCoord = ~ibuff[0];
				aYCoord = ~ibuff[1];
				bXCoord = ~ibuff[2];
				bYCoord = ~ibuff[3];
				beginPlay(data);
				break;
			}
			if (!TIMER_checkTimer(TILTMOVE_TIMER_INSTRUCTIONS)) {
				prevMEMSX = (MEMS_GetInfo())->OutX_F4;
				prevMEMSY = (MEMS_GetInfo())->OutY_F4;
			}
			else if (data->isHost == TRUE) {
				if (data->mode != Game_SinglePlayer) {
					//Send these co-ordinates to the client
					u8 buff[sizeof(int) * 4];
					int * ibuff = (int*) buff;
					ibuff[0] = ~aXCoord;
					ibuff[1] = ~aYCoord;
					ibuff[2] = ~bXCoord;
					ibuff[3] = ~bYCoord;
					buff[sizeof(float) * 4] = 0;
					//((u8 *) buff)[4 * sizeof(int) + 1] = '\0';
					NET_TransmitStringPacket(42, (u8 * )buff);
				}
				beginPlay(data);
				TIMER_disableTimer(TILTMOVE_TIMER_INSTRUCTIONS);
			}
			break;
		case TM_State_PLAYING:
			play(data);
			break;
	}
	

}

static void play(struct GameData * data) {
	// Have we run out of time?
	if (TIMER_checkTimer(TILTMOVE_TIMER_GAME) && data->isHost == TRUE) {
		end(data);
		data->code = gameStatus_Fail;
		return;
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

	if (data->mode == Game_CoOp) {
		
		if (!(NET_GetFlags() & NETTICK_FLAG_TX)) {
			u8 buff[2];
			buff[1] = '\0';
			if (data->isHost)
				buff[0] = (u8) ballXCoord;
			else
				buff[0] = (u8) ballYCoord;
			NET_TransmitStringPacket(PACKET_gameData, buff);
		}

		//In CoOp mode, check for incoming packets and deal with them.
		if (NET_GetFlags() & NETTICK_FLAG_RX && NET_GetPacketType() == PACKET_gameData) {
			u8 buff[2];
			NET_GetPacketData(buff);
			float coord = (float) buff[0];
			if (data->isHost) {
				ballYCoord = (coord > 0 && coord < SCREEN_HEIGHT ? coord : ballYCoord);
			}
			else {
				ballXCoord = (coord > 0 && coord < SCREEN_WIDTH ? coord : ballXCoord);
			}
		}
	
		if (data->isHost == TRUE)
			deltaMEMSY = 0;
		else
			deltaMEMSX = 0;
	}

	// Update ball position.
	ballXCoord += (deltaMEMSX / 1000.0);
	ballYCoord += (deltaMEMSY / 1000.0);
	
	// Are we still within the track?
	/*
	 * If we look at the lines going around the track clockwise,
	 * the ball must always be to its left, regardless of the
	 * gradient of the track.
	 */
	bool inside = 1; // Are we outside?
	int position = -1; // -1 when on right.

	int i;
	for (i = 0; i < 6 - 1; i++) {
		position = sign( (xCoords[i + 1] - xCoords[i])*(ballYCoord - yCoords[i])
					   - (yCoords[i + 1] - yCoords[i])*(ballXCoord - xCoords[i]) );
		inside = inside & (position == 1);
	}

	position = sign( (xCoords[0] - xCoords[5])*(ballYCoord - yCoords[5])
				   - (yCoords[0] - yCoords[5])*(ballXCoord - xCoords[5]) );
	inside = inside & (position == 1);
	
	if (!inside && data->isHost == TRUE) {
		data->code = gameStatus_Fail;
		end(data);
		return;
	}
	
	// Are we at B yet?
	if ( absolute( ballXCoord - bXCoord) <= radius &&
		 absolute( ballYCoord - bYCoord) <= radius &&
	     data->isHost == TRUE) {
		
		data->code = gameStatus_Success;
		data->score = TIMER_ticksLeft(TILTMOVE_TIMER_GAME);
		end(data);
	}

	TIMER_drawTicker(TILTMOVE_TIMER_GAME);
}

__attribute__((section(".rodata"))) static void init(struct GameData * data) {

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

	//Host sets up coordinates
	if (data->isHost == TRUE) {
		// Initialise random number generator. (do we want to do this?)
		/*if (!randomInit) {
			init_rand(23);
			randomInit = 1;
		}*/
		
		aXCoord = aYCoord = bXCoord = bYCoord = 0;

		while (absolute(aXCoord - bXCoord) < (radius * 5.0) && absolute(aYCoord - bYCoord) < (radius * 5.0)) {
			// Set start and finish to random locations.
			aXCoord = (rand_cmwc() % (SCREEN_WIDTH - 30)) + 15;
			aYCoord = (rand_cmwc() % 30) + 15;
			bXCoord = (rand_cmwc() % (SCREEN_WIDTH - 30)) + 15;
			bYCoord = SCREEN_HEIGHT - ((rand_cmwc() % 30) + 15);
			
			// Maybe flip.
			if (rand_bool() == TRUE) {
				int temp = aXCoord;
				aXCoord = aYCoord;
				aYCoord = temp;
				
				temp = bXCoord;
				bXCoord = bYCoord;
				bYCoord = temp;
			}
		}	
							   
	// Initialise timers.
	if (data->isHost == TRUE) TIMER_initTimer(TILTMOVE_TIMER_INSTRUCTIONS, TIME_SECOND * 2);
	TIMER_disableTimer(TILTMOVE_TIMER_DRAWING); // This one is disabled for initial drawing.
	
	}
	state = TM_State_INSTRUCTIONS;

}

static void beginPlay(struct GameData * data) {

	//So THIS is why they taught us vectors!
	float size = data->mode == Game_CoOp ? 20 : 10;
	float ux = size * cos(atan2(bYCoord - aYCoord, bXCoord - aXCoord));
	float uy = size * sin(atan2(bYCoord - aYCoord, bXCoord - aXCoord));
	
	xCoords[0] = aXCoord - uy;
	xCoords[1] = aXCoord - ux;
	xCoords[2] = aXCoord + uy;
	xCoords[3] = bXCoord + uy;
	xCoords[4] = bXCoord + ux;
	xCoords[5] = bXCoord - uy;

	yCoords[0] = aYCoord + ux;
	yCoords[1] = aYCoord - uy;
	yCoords[2] = aYCoord - ux; 
	yCoords[3] = bYCoord - ux;
	yCoords[4] = bYCoord + uy;
	yCoords[5] = bYCoord + ux;

	// Set game status to "in progress".
	data->code = gameStatus_InProgress;
	
	// Set up drawing.
	DRAW_SetTextColor(RGB_BLUE);
	DRAW_SetBGndColor(RGB_WHITE);
	
	// Set ball initial position to A.
	ballXCoord = aXCoord;
	ballYCoord = aYCoord;

	TIMER_initTimer(TILTMOVE_TIMER_GAME, TIME_SECOND * (data->mode == Game_SinglePlayer ? 5 : 15));
	state = TM_State_PLAYING;
	
}

__attribute__((section(".rodata"))) static void end(struct GameData * data) {
	state = TM_State_START;
	randomInit = 0;
}