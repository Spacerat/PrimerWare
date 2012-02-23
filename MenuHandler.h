#ifndef MENUHANDLER_H_GUARD
#define MENUHANDLER_H_GUARD

enum MenuCode {
	MenuCode_SinglePlayer = 0,
	MenuCode_TwoPlayerCoOp = 1,
	MenuCode_TwoPlayerVs = 2,
	MenuCode_Nothing = 3
};

enum MenuCode MENUHANDLER_run(void);
	void MENUHANDLER_SinglePlayer(void);
	void MENUHANDLER_MultiPlayer(void);
	void MENUHANDLER_CoOp(void);
	void MENUHANDLER_Vs(void);
#endif