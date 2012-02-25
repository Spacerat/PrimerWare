/******************************************************************************
*
* File Name          :  Touchscreen.c
* Description        :  Touchscreen helper functions.
*
*******************************************************************************/

#include "circle_api.h"

#include "Touchscreen.h"

struct TouchEvent TOUCH_clickEvent(void) {
	struct TouchEvent touchEvent;
	static bool lastTouch = 0;
	u16 clickPosition = 0;
	
	if (TOUCHSCR_IsPressed())
		if (lastTouch) {
			lastTouch = 1;
			touchEvent.type = TouchType_StillPressed;
		} else {
			lastTouch = 1;
			touchEvent.type = TouchType_Pressed;
		}
	else
		if (lastTouch) {
			lastTouch = 0;
			touchEvent.type = TouchType_Depressed;
		} else {
			lastTouch = 0;
			touchEvent.type = TouchType_StillDepressed;
		}
		
	touchEvent.position = clickPosition;
		
	return touchEvent;
}