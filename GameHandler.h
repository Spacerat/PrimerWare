#ifndef GAMEHANDLER_H_GUARD
#define GAMEHANDLER_H_GUARD

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

struct GameStatus {
	enum GameStatusCode code;
	int score;
};

struct GameData {
	enum GameModeCode mode; // i.e. sp, coop, versus
	u8 isHost;
};

typedef struct GameStatus (*gameRunFunction)(struct GameData * data);

/* Constants -----------------------------------------------------------------*/
#define NUMSINGLEGAMES 3 // Total number of single player games.
#define NUMCOOPGAMES 0 // Total number of co-op games.
#define NUMVSGAMES 0 // Total number of versus games.
#define ROUNDLENGTH 3 // Number of games in a round.

#endif