/******************************************************************************
*
* File Name          :  GameDraw.c
* Description        :  Functions for drawing various game things, such as the
						next stage screen, etc.
*
*******************************************************************************/

#include "circle_api.h"

__attribute__((section(".rodata"))) void GAMEDRAW_stageN(unsigned int n) {
	char stageNoString[2];
	
	UTIL_uint2str(stageNoString, n, 2, 0);
	
	char stageTextString[8+1] = "Stage "; // String to display.
	stageTextString[6] = stageNoString[0];
	stageTextString[7] = stageNoString[1];
	
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, stageTextString, ALL_SCREEN, 1, 1);
}

__attribute__((section(".rodata"))) void GAMEDRAW_stageSuccess(void) {
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, "Stage Cleared!", ALL_SCREEN, 1, 1);
}
__attribute__((section(".rodata"))) void GAMEDRAW_stageFail(void) {
	//DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, "Stage Failed D:", ALL_SCREEN, 1, 1);
}
__attribute__((section(".rodata"))) void GAMEDRAW_roundFinished(int score, int lives) {
	char scoreString[8];
	char livesString[1];

	char scoreTextString[7+8+1] = "Score: "; // Score string to display.
	char livesTextString[12+1+1] = "Lives left: "; // Lives string to display.
	
	UTIL_int2str(scoreTextString + 6, score, 8, 1);
	UTIL_int2str(livesTextString + 11, lives, 1, 1);
	
	scoreTextString[7 + 8] = '\0';
	livesTextString[12 + 1] = '\0';
	
	/*
	int i;
	for (i = 0; i < 8; i++) scoreTextString[i + 7] = scoreString[i];
	for (i = 0; i < 1; i++) livesTextString[i + 12] = livesString[i];
		
	scoreTextString[7+8] = '\0';
	livesTextString[12+1] = '\0';
	
	*/
	
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 75, "Round Over", ALL_SCREEN, 1, 1);
	DRAW_DisplayStringWithMode(0, 60, scoreTextString, ALL_SCREEN, 1, 1);
	DRAW_DisplayStringWithMode(0, 45, livesTextString, ALL_SCREEN, 1, 1);
}