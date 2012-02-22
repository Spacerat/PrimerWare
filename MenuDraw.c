/******************************************************************************
*
* File Name          :  MenuDraw.c
* Description        :  Contains functions for drawing certain components of 
*						the main menu.
*
*******************************************************************************/

#include "circle_api.h"

/*
 * Set up drawing stuff.
 */
void MENUDRAW_setupDrawing(void) {
	DRAW_SetCharMagniCoeff(1);
	DRAW_SetTextColor(RGB_WHITE);
	DRAW_SetBGndColor(RGB_BLUE);
}

void MENUDRAW_drawMenu(void) {
	MENUDRAW_setupDrawing();
	MENUDRAW_drawTitle();
	MENUDRAW_drawPlayersMenu();
}

void MENUDRAW_drawTitle(void) {
	DRAW_DisplayStringWithMode(0, 100, "PrimerWare!", ALL_SCREEN, 0, 1);
}
void MENUDRAW_drawPlayersMenu(void) {
	// Screen /space/ is 127x128.

	MENUDRAW_clearButtonAreas();
	
	// Draw box for "Single Player".
	DRAW_DisplayStringWithMode(0, 64, "Single Player", ALL_SCREEN, 1, 1);
	LCD_DrawRect(16, 60, 93, 20, RGB_BLUE);
	
	// Draw box for "Two Players".
	DRAW_DisplayStringWithMode(0, 34, "Two Players", ALL_SCREEN, 1, 1);
	LCD_DrawRect(16, 30, 93, 20, RGB_BLUE);
}

/*
 * Clears the button areas by filling with white.
 */
void MENUDRAW_clearButtonAreas(void) {
	LCD_FillRect(16, 60, 93, 20, RGB_WHITE);
	LCD_FillRect(16, 30, 93, 20, RGB_WHITE);	
}

void MENUDRAW_drawTwoPlayerGameTypeMenu(void) {
	MENUDRAW_clearButtonAreas();
	
	// Draw box for "Co-op".
	DRAW_DisplayStringWithMode(0, 64, "Co-op", ALL_SCREEN, 1, 1);
	LCD_DrawRect(16, 60, 93, 20, RGB_BLUE);
	
	// Draw box for "Versus".
	DRAW_DisplayStringWithMode(0, 34, "Versus", ALL_SCREEN, 1, 1);
	LCD_DrawRect(16, 30, 93, 20, RGB_BLUE);
}