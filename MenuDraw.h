#ifndef MENUDRAW_H_GUARD
#define MENUDRAW_H_GUARD
void MENUDRAW_setupDrawing(void);
void MENUDRAW_drawMenu(void);
	void MENUDRAW_drawTitle(void);
	void MENUDRAW_drawPlayersMenu(void);
	void MENUDRAW_drawTwoPlayerGameTypeMenu(void);
void MENUDRAW_clearButtonAreas(void);

#define MENUDRAW_BOXES_XCOORD 16
#define MENUDRAW_TOPBOX_YCOORD 60
#define MENUDRAW_BTMBOX_YCOORD 30
#define MENUDRAW_BOXES_WIDTH 93
#define MENUDRAW_BOXES_HEIGHT 20
#endif