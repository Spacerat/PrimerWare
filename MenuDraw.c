/******************************************************************************
*
* File Name          :  MenuDraw.c
* Description        :  Contains functions for frawing certain components of 
*						the main menu.
*
*******************************************************************************/

#include "circle_api.h"

void drawMenu(void) {
	drawTitle();
	//drawPlayersMenu();
}

void drawTitle(void) {
	DRAW_SetCharMagniCoeff(1);
	DRAW_SetTextColor(RGB_WHITE);
	DRAW_SetBGndColor(RGB_BLUE);

	DRAW_DisplayStringWithMode(0, 100, "PrimerWare!", ALL_SCREEN, 0, 1);
}
void drawPlayersMenu(void);
void drawTwoPlayerGameTypeMenu(void);