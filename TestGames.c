/******************************************************************************
*
* File Name          :  TestGames.c
* Description        :  Test games.
*
*******************************************************************************/

#include "circle_api.h"

#include "GameHandler.h"

struct GameStatus testRun1(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 1",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun2(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 2",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun3(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 3",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}