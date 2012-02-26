/******************************************************************************
*
* File Name          :  Touchscreen.c
* Description        :  Touchscreen helper functions.
*
*******************************************************************************/

#include "circle_api.h"

#include "Touchscreen.h"

//Todo: maybe take a TouchEvent pointer as an argument instead?
struct TouchEvent TOUCH_clickEvent(void) {
	static struct TouchEvent touchEvent;
	static bool lastTouch = 0;

	if (TOUCHSCR_IsPressed())
		if (lastTouch) {
			lastTouch = 1;
			touchEvent.type = TouchType_StillPressed;
			touchEvent.position = TOUCHSCR_GetPos();
		} else {
			lastTouch = 1;
			touchEvent.type = TouchType_Pressed;
			touchEvent.position = TOUCHSCR_GetPos();
		}
	else
		if (lastTouch) {
			lastTouch = 0;
			touchEvent.type = TouchType_Depressed;
		} else {
			lastTouch = 0;
			touchEvent.type = TouchType_StillDepressed;
		}
		
	return touchEvent;
}