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

static bool menuDrawn = 0; // Has the (players) menu been drawn yet?


/*
 * Set the menu as drawn (or not) so it will redraw.
 */
__attribute__((section(".rodata"))) enum MenuCode MENUHANDLER_setDrawn(bool drawn) {
	menuDrawn = drawn;
}

/*
 * Main menu call.
 */
__attribute__((section(".rodata"))) enum MenuCode MENUHANDLER_run() {
	static bool playersMenu = 1; // Are we on the players menu?
	
	// Draw the menu if it hasn't already been drawn.
	if (!menuDrawn) {
		MENUDRAW_drawMenu();
		menuDrawn = 1;
	}
	
	// Check if the user has touched in the screen. If the touch was within a
	// button, call the function for that button.
	struct TouchEvent t = TOUCH_clickEvent();
	
	if (t.type == TouchType_Depressed) {
		u8 xPos = (u8)(t.position);
        u8 yPos = (u8)(t.position>>8);
          	
		// Top button pressed.
		if (xPos >= MENUDRAW_BOXES_XCOORD &&
			xPos <= MENUDRAW_BOXES_XCOORD + MENUDRAW_BOXES_WIDTH &&
			yPos >= MENUDRAW_TOPBOX_YCOORD &&
			yPos <= MENUDRAW_TOPBOX_YCOORD + MENUDRAW_BOXES_HEIGHT) {
						
			if (playersMenu) return MenuCode_SinglePlayer;
			else {
				playersMenu = 1;
				return MenuCode_TwoPlayerCoOp;
			}
		// Bottom button pressed.
		} else if (xPos >= MENUDRAW_BOXES_XCOORD &&
				   xPos <= MENUDRAW_BOXES_XCOORD + MENUDRAW_BOXES_WIDTH &&
				   yPos >= MENUDRAW_BTMBOX_YCOORD &&
				   yPos <= MENUDRAW_BTMBOX_YCOORD + MENUDRAW_BOXES_HEIGHT) {
			if (playersMenu) {
				playersMenu = 0;
				MENUHANDLER_MultiPlayer();
			} else {
				playersMenu = 1;
				return MenuCode_TwoPlayerVs;
			}
		}
	}
	
	return MenuCode_Nothing;
}

__attribute__((section(".rodata"))) void MENUHANDLER_SinglePlayer(void) { }
__attribute__((section(".rodata"))) void MENUHANDLER_MultiPlayer(void) { 
	MENUDRAW_drawTwoPlayerGameTypeMenu();
}
__attribute__((section(".rodata"))) void MENUHANDLER_CoOp(void) { }
__attribute__((section(".rodata"))) void MENUHANDLER_Vs(void) { }
