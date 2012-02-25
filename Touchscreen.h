#ifndef TOUCHSCREEN_H_GUARD
#define TOUCHSCREEN_H_GUARD

enum TouchType {
	TouchType_StillDepressed = 0,
	TouchType_Pressed = 1,
	TouchType_Depressed = 2,
	TouchType_StillPressed = 3
};

struct TouchEvent {
	enum TouchType type;
	u16 position;
};

struct TouchEvent TOUCH_clickEvent(void);

#endif