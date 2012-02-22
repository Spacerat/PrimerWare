/******************************************************************************
*
* File Name          :  MenuHandler.c
* Description        :  Code for menu functionality.
*
*******************************************************************************/

#include "circle_api.h"
#include "MenuHandler.h"

#include "MenuDraw.h"

/*
 * Main menu call.
 */
enum MenuCode MENUHANDLER_run(void) {
	static bool menuDrawn = 0; // Has the (players) menu been drawn yet?
	static bool playersMenu = 1; // Are we on the players menu?
	
	// Draw the menu if it hasn't already been drawn.
	if (!menuDrawn) {
		MENUDRAW_drawMenu();
		menuDrawn = 1;
	}
	
	// Check if the user has touched in the screen. If the touch was within a
	// button, call the function for that button.
	if (TOUCHSCR_IsPressed()) {
		  u16 uStylusPos = TOUCHSCR_GetPos();
          u8  xPos = (u8)uStylusPos;
          u8  yPos = (u8)(uStylusPos>>8);
          	
		// Top button pressed.
		if (xPos >= 16 &&
			xPos <= 93 + 16 &&
			yPos >= 60 &&
			yPos <= 20 + 60) {
			if (playersMenu) MENUHANDLER_SinglePlayer();
			else MENUHANDLER_CoOp();
		// Bottom button pressed.
		} else if (xPos >= 16 &&
				   xPos <= 93 + 16 &&
				   yPos >= 30 &&
				   yPos <= 20 + 30) {
			if (playersMenu) {
				playersMenu = 0;
				MENUHANDLER_MultiPlayer();
			} else { MENUHANDLER_Vs();
			}
		}
	}
}

void MENUHANDLER_SinglePlayer(void) { }
void MENUHANDLER_MultiPlayer(void) { 
	MENUDRAW_drawTwoPlayerGameTypeMenu();
}
void MENUHANDLER_CoOp(void) { }
void MENUHANDLER_Vs(void) { }
