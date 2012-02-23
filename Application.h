#ifndef APPLICATION_H_GUARD
#define APPLICATION_H_GUARD
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
#endif