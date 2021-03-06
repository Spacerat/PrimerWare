#ifndef GAMEHANDLER_H_GUARD
#define GAMEHANDLER_H_GUARD

#include "circle_api.h"

enum GameStatusCode {
	gameStatus_InProgress = 0,
	gameStatus_Success = 1,
	gameStatus_Fail = 2
};

enum GameModeCode {
	Game_SinglePlayer = 0,
	Game_CoOp = 1,
	Game_Vs = 2,
	Game_None = 3
};

struct GameData {
	enum GameModeCode mode; // i.e. sp, coop, versus
	enum GameStatusCode code;
	bool isHost;
	u16 score;
};

typedef void (*gameRunFunction)(struct GameData * data);

/* Constants -----------------------------------------------------------------*/


#endif