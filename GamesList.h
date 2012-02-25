#ifndef GAMESLIST_H_GUARD
#define GAMESLIST_H_GUARD

#include "GameHandler.h"
#include "minigames/TestGames.h"
#include "minigames/tappy.h"


gameRunFunction minigamesSinglePlayer[] = { minigame_tappy, testRun1, testRun2, testRun3 };
gameRunFunction minigamesCoOp[] = { minigame_tappy, testRun1, testRun2, testRun3 };
gameRunFunction minigamesVs[] = { minigame_tappy, testRun1, testRun2, testRun3 };

#endif