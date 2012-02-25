/******************************************************************************
*
* File Name          :  Application.c
* Description        :  Main application source file.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle_api.h"

#include "Application.h"
#include "MenuHandler.h"
#include "RandNo.h"
#include "GameDraw.h"
#include "Timer.h"
#include "Net.h"

/* Constants -----------------------------------------------------------------*/
#define NUMSINGLEGAMES 3 // Total number of single player games.
#define NUMCOOPGAMES 0 // Total number of co-op games.
#define NUMVSGAMES 0 // Total number of versus games.
#define ROUNDLENGTH 3 // Number of games in a round.

/* Private defines -----------------------------------------------------------*/

// The following should be the minimal CircleOS version needed by your 
// application.
#define NEEDEDVERSION "V 4.0"

/* Private functions ---------------------------------------------------------*/
static enum MENU_code MsgVersion(void);

/* Public variables ----------------------------------------------------------*/
const char Application_Name[8+1] = {"Pri.War."};      // Max 8 characters

/* Game function shizzles ----------------------------------------------------*/
struct GameStatus testRun1(void);
struct GameStatus testRun2(void);
struct GameStatus testRun3(void);
const gameRunFunction minigamesSinglePlayer[] = { testRun1, testRun2,
												  testRun3 };
const gameRunFunction minigamesCoOp[] = { testRun1, testRun2, testRun3 };
const gameRunFunction minigamesVs[] = { testRun1, testRun2, testRun3 };												  

/*******************************************************************************
* Function Name  : Application_Ini
* Description    : Initialization function of Circle_App. This function will be
*                  called only once by CircleOS.
* Input          : None
* Return         : MENU_CONTINUE_COMMAND
*******************************************************************************/
enum MENU_code Application_Ini(void) {
    // Ensure that the current OS version is recent enough
    if(strcmp(UTIL_GetVersion(), NEEDEDVERSION) < 0) {
        return MsgVersion();
    }
    
    // This application manages all the screen. 
    // If you don't reset the offset on EvoPrimer, the screen will be reduced
    // to 128x128 pixels for Primer2 compatibility
    //LCD_SetOffset(OFFSET_OFF);
	
	// Disable screen rotation.
	LCD_SetRotateScreen(0);
    
    // Set speed.
	UTIL_SetPll(SPEED_VERY_HIGH);
    MENU_SetAppliDivider(10);

    return MENU_CONTINUE_COMMAND;
    }

/*******************************************************************************
* Function Name  : Application_Handler
* Description    : Management of the Circle_App. This function will be called
*                  every millisecond by CircleOS while it returns MENU_CONTINUE.
* Input          : None
* Return         : MENU_CONTINUE
*******************************************************************************/
enum MENU_code Application_Handler(void)
    {
    // This routine will get called repeatedly by CircleOS, until we
    // return MENU_LEAVE

	// State for the game handler.
	static bool atMenu = 1; // Are we supposed to be at the menu?
	static gameRunFunction minigameArray[ROUNDLENGTH]; // Holds current 
													   // minigames.
	static int currentMinigame = 0; // Index in the array of the current 
									// pointer.
	static int score = 0; // Current total score.
	static int lives = 3; // Number of lives remaining.
	static int amServer = 0; // Am I the server (multiplayer only of course)?
	
	// Make sure the timers count up!
	TIMER_tickTimers();
		
	if (atMenu) {
		enum MenuCode menuCode = MENUHANDLER_run(); // Handle menu and get code.
		
		// If we have chosen a game type...
		if (menuCode != MenuCode_Nothing) {			
			// Initialise the array of games.
			init_rand(123);
			
			// Populate the minigame array according to game type.
			gameRunFunction* theMinigames;
			if (menuCode == MenuCode_SinglePlayer)
				theMinigames = minigamesSinglePlayer;
			else if (menuCode == MenuCode_TwoPlayerCoOp)
				theMinigames = minigamesCoOp;
			else
				theMinigames = minigamesVs;
			
			int i = 0;
			for (i = 0; i < ROUNDLENGTH; i++)
				minigameArray[i] = theMinigames[rand_cmwc() % ROUNDLENGTH];
			
			// If a multiplayer game was started, set up the multiplayer.
			if (menuCode != MenuCode_SinglePlayer) {
				// TODO: Implement multiplayer handling.
				DRAW_DisplayStringWithMode(0,
										   0,
										   "Multiplayah!",
										   ALL_SCREEN, 0, 1);
			}
			
			// Change from menu mode to game mode.
			atMenu = 0;
		}
	} else {
		// Display the stage screen if we're about to start a game.
		
		if (!TIMER_isEnabled(0)) {
			TIMER_initTimer(0, 200);
			GAMEDRAW_stageN(currentMinigame + 1);
		}
		
		if (!TIMER_checkTimer(0)) return MENU_CONTINUE;
		
		/*static int counter = 0; // Counter for timing.
		if (counter == 0) {
			GAMEDRAW_stageN(currentMinigame + 1);
		}
		if (counter < 200) {
			counter++;
			return MENU_CONTINUE;
		}*/
		
		// Run the current game and get its status.
		struct GameStatus gameStatus = minigameArray[currentMinigame]();
			
		// Handle game results.
		if (gameStatus.code == gameStatus_Success) {
			score += gameStatus.score;
			currentMinigame++;
			TIMER_disableTimer(0);
		} else if (gameStatus.code == gameStatus_Fail) {
			lives--;
			currentMinigame++;
			TIMER_disableTimer(0);
		}
		
		// If you die or finish, display stuff then exit.
		if (lives == 0 || currentMinigame == ROUNDLENGTH) {
			GAMEDRAW_roundFinished(score, lives);
			
			atMenu = 1;
			lives = 3;
			score = 0;
			currentMinigame = 0;
		}
	}
		
    // If the button is pressed, the application is exited
    if (BUTTON_GetState() == BUTTON_PUSHED) {
        BUTTON_WaitForRelease();
        return MENU_Quit();
    }

    return MENU_CONTINUE;   // Returning MENU_LEAVE will quit to CircleOS
}

/*******************************************************************************
* Function Name  : MsgVersion
* Description    : Displays the current CircleOS version and the version needed
*                  exit to main menu after 4 seconds
* Input          : None
* Return         : MENU_REFRESH
*******************************************************************************/
static enum MENU_code MsgVersion(void)
    {
    u8 hh, mm, ss, ss2;

    DRAW_DisplayString( 5,  60, "CircleOS", 17 );
    DRAW_DisplayString( 80, 60, UTIL_GetVersion(), 6 );
    DRAW_DisplayString( 5,  34, NEEDEDVERSION, 6 );
    DRAW_DisplayString( 50, 34, " required", 12 );

    RTC_GetTime( &hh, &mm, &ss );
    ss = ss + 4;
    ss = ss % 60;

    do
        {
        RTC_GetTime( &hh, &mm, &ss2 );
        }
    while ( ss2 != ss );           // do while < 4 seconds

    DRAW_Clear();
    return MENU_REFRESH;
    }

// ----------------------- TEST SHIZZLE!!! -----------------------	
	
struct GameStatus testRun1(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 1",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun2(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 2",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun3(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   25,
							   "Test 3",
							   ALL_SCREEN, 0, 1);
	
	// TIMING CODE!
	static int count = 0;
	char countString[8];
	
	UTIL_int2str(countString, count, 8, 1);
	DRAW_DisplayStringWithMode(0,
							   10,
							   countString,
							   ALL_SCREEN, 0, 1);
	
	count++;
	// END TIMING CODE!
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}