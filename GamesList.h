#ifndef GAMESLIST_H_GUARD
#define GAMESLIST_H_GUARD

#define NUMSINGLEGAMES 1 // Total number of single player games.
#define NUMCOOPGAMES 0 // Total number of co-op games.
#define NUMVSGAMES 0 // Total number of versus games.
#define ROUNDLENGTH 1 // Number of games in a round.

#include "GameHandler.h"
#include "minigames/TestGames.h"
#include "minigames/BalloonInflate.h"


gameRunFunction minigamesSinglePlayer[] = { BalloonInflate_run };
gameRunFunction minigamesCoOp[] = { BalloonInflate_run };
gameRunFunction minigamesVs[] = { BalloonInflate_run };

#endif