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

/* Private defines -----------------------------------------------------------*/

// The following should be the minimal CircleOS version needed by your 
// application.
#define NEEDEDVERSION "V 4.0"

/* Private functions ---------------------------------------------------------*/
static enum MENU_code MsgVersion(void);

/* Public variables ----------------------------------------------------------*/
const char Application_Name[8+1] = {"Pri.War."};      // Max 8 characters
const int numGames = 3; // Number of games.

/* Game function shizzles ----------------------------------------------------*/
struct GameStatus testRun1(void);
struct GameStatus testRun2(void);
struct GameStatus testRun3(void);
const gameRunFunction minigames[] = { testRun1, testRun2, testRun3 };

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
    LCD_SetOffset(OFFSET_OFF);
	
	// Disable screen rotation.
	LCD_SetRotateScreen(0);
    
    // Set speed.
	UTIL_SetPll(SPEED_VERY_HIGH);
    MENU_SetAppliDivider(10);
	
	// Run the MenuHandler.
	MENUHANDLER_run();

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
	static const int sessionLength = 3; // Number of games per session.
	static gameRunFunction minigameArray[3]; // Array of pointers to
														 // minigame run functions.
	static int currentMinigame = 0; // Index in the array of the current pointer.
	static int score = 0; // Current total score.
	static int lives = 3; // Number of lives remaining.
	static int amServer = 0; // Am I the server (multiplayer only of course)?
		
	if (atMenu) {
		enum MenuCode menuCode = MENUHANDLER_run(); // Handle menu and get code.
		
		// If we have chosen a game type...
		if (menuCode != MenuCode_Nothing) {			
			// Initialise the array of games.
			init_rand(123);
			
			int i = 0;
			for (i = 0; i < sessionLength; i++)
				minigameArray[i] = minigames[rand_cmwc() % numGames];
			
			// If a multiplayer game was started, set up the multiplayer.
			if (menuCode != MenuCode_SinglePlayer) {
				// TODO: Implement multiplayer handling.
				DRAW_DisplayStringWithMode(0,
										   0,
										   "Multiplayah!",
										   ALL_SCREEN, 1, 1);
			}
			
			// TODO: Display start notification.
			
			// Change from menu mode to game mode.
			atMenu = 0;
		}
	} else {
		// Run the current game and get its status.
		struct GameStatus gameStatus = minigameArray[currentMinigame]();
			
		// Handle game results.
		if (gameStatus.code == gameStatus_Success) {
			score += gameStatus.score;
			currentMinigame++;
		} else if (gameStatus.code == gameStatus_Fail) {
			lives--;
			currentMinigame++;
		}
		
		// If you die or finish, display stuff then exit.
		if (lives == 0 || currentMinigame == sessionLength) {
			// TODO: Display score, etc.
			
			atMenu = 1;
			lives = 3;
			score = 0;
			currentMinigame = 0;
		}
	}
		
    // If the button is pressed, the application is exited
    if (BUTTON_GetState() == BUTTON_PUSHED)
        {
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
							   10,
							   "Test 1",
							   ALL_SCREEN, 1, 1);
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun2(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   10,
							   "Test 2",
							   ALL_SCREEN, 1, 1);
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}

struct GameStatus testRun3(void) {
	static struct GameStatus status;
	status.code = gameStatus_InProgress;
	status.score = 0;
	
	DRAW_Clear();
	DRAW_DisplayStringWithMode(0,
							   10,
							   "Test 3",
							   ALL_SCREEN, 1, 1);
	
	if (TOUCHSCR_IsPressed()) status.code = gameStatus_Success;
		
	return status;
}