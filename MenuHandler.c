/******************************************************************************
*
* File Name          :  MenuHandler.c
* Description        :  Code for menu functionality.
*
*******************************************************************************/

#include "circle_api.h"

#include "MenuHandler.h"
#include "MenuDraw.h"
#include "Touchscreen.h"

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
	//struct TouchEvent touchEvent = TOUCH_clickEvent();
	
	if (TOUCHSCR_IsPressed() == 1) {
		u16 pos = TOUCHSCR_GetPos();
        u8  xPos = (u8)(pos);
        u8  yPos = (u8)(pos>>8);
          	
		// Top button pressed.
		if (xPos >= MENUDRAW_BOXES_XCOORD &&
			xPos <= MENUDRAW_BOXES_XCOORD + MENUDRAW_BOXES_WIDTH &&
			yPos >= MENUDRAW_TOPBOX_YCOORD &&
			yPos <= MENUDRAW_TOPBOX_YCOORD + MENUDRAW_BOXES_HEIGHT) {
			
			menuDrawn = 0;	
			
			if (playersMenu) return MenuCode_SinglePlayer;
			else return MenuCode_TwoPlayerCoOp;
		// Bottom button pressed.
		} else if (xPos >= MENUDRAW_BOXES_XCOORD &&
				   xPos <= MENUDRAW_BOXES_XCOORD + MENUDRAW_BOXES_WIDTH &&
				   yPos >= MENUDRAW_BTMBOX_YCOORD &&
				   yPos <= MENUDRAW_BTMBOX_YCOORD + MENUDRAW_BOXES_HEIGHT) {
			if (playersMenu) {
				playersMenu = 0;
				MENUHANDLER_MultiPlayer();
			} else {
				menuDrawn = 0;
				return MenuCode_TwoPlayerVs;
			}
		}
	}
	
	return MenuCode_Nothing;
}

void MENUHANDLER_SinglePlayer(void) { }
void MENUHANDLER_MultiPlayer(void) { 
	MENUDRAW_drawTwoPlayerGameTypeMenu();
}
void MENUHANDLER_CoOp(void) { }
void MENUHANDLER_Vs(void) { }
