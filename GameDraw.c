/******************************************************************************
*
* File Name          :  GameDraw.c
* Description        :  Functions for drawing various game things, such as the
						next stage screen, etc.
*
*******************************************************************************/

#include "circle_api.h"

void GAMEDRAW_stageN(unsigned int n) {
	char stageNoString[2];
	
	UTIL_uint2str(stageNoString, n, 2, 0);
	
	char stageTextString[8+1] = "Stage "; // String to display.
	stageTextString[6] = stageNoString[0];
	stageTextString[7] = stageNoString[1];
	
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, stageTextString, ALL_SCREEN, 1, 1);
}

void GAMEDRAW_stageSuccess(void) {
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, "Stage Cleared!", ALL_SCREEN, 1, 1);
}
void GAMEDRAW_stageFail(void) {
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 60, "Stage Failed D:", ALL_SCREEN, 1, 1);
}
void GAMEDRAW_roundFinished(int score, int lives) {
	char scoreString[8];
	char livesString[1];
	
	UTIL_int2str(scoreString, score, 8, 1);
	UTIL_int2str(livesString, lives, 1, 0);
	
	char scoreTextString[7+8+1] = "Score: "; // Score string to display.
	char livesTextString[11+1+1] = "Lives left: "; // Lives string to display.
	
	int i;
	for (i = 7; i < 7+8; i++) scoreTextString[i] = scoreString[i - 7];
		
	livesTextString[11] = livesString[0];
	
	DRAW_Clear();
	
	DRAW_DisplayStringWithMode(0, 75, "Round Over", ALL_SCREEN, 1, 1);
	DRAW_DisplayStringWithMode(0, 60, scoreTextString, ALL_SCREEN, 1, 1);
	DRAW_DisplayStringWithMode(0, 45, livesTextString, ALL_SCREEN, 1, 1);
}