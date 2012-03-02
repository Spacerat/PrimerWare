/******************************************************************************
*
* File Name          :  TestGames.c
* Description        :  Test games.
*
*******************************************************************************/

#include "TestGames.h"
#include "circle_api.h"
#include "../GameHandler.h"
#include "../Touchscreen.h"

__attribute__((section(".rodata"))) void testRun1(struct GameData * data) {
	data->code = gameStatus_InProgress;
	data->score = 0;
	
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
	
	if (TOUCH_clickEvent().type == TouchType_Depressed)
		data->code = gameStatus_Fail;

}

__attribute__((section(".rodata"))) void testRun2(struct GameData * data) {
	data->code = gameStatus_InProgress;
	data->score = 0;
	
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
	
	if (TOUCH_clickEvent().type == TouchType_Depressed)
		data->code = gameStatus_Success;
}

__attribute__((section(".rodata"))) void testRun3(struct GameData * data) {
	data->code = gameStatus_InProgress;
	data->score = 0;
	
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
	
	if (TOUCH_clickEvent().type == TouchType_Depressed)
		data->code = gameStatus_Success;
		
}