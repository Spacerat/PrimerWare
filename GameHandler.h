#ifndef GAMEHANDLER_H_GUARD
#define GAMEHANDLER_H_GUARD

enum GameStatusCode {
	gameStatus_InProgress = 0,
	gameStatus_Success = 1,
	gameStatus_Fail = 2
};

struct GameStatus {
	enum GameStatusCode code;
	int score;
};

typedef struct GameStatus (*gameRunFunction)(void);

/* Constants -----------------------------------------------------------------*/
#define NUMSINGLEGAMES 3 // Total number of single player games.
#define NUMCOOPGAMES 0 // Total number of co-op games.
#define NUMVSGAMES 0 // Total number of versus games.
#define ROUNDLENGTH 3 // Number of games in a round.

#endif