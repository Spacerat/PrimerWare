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
#include "GameHandler.h"
#include "GamesList.h"
#include "Touchscreen.h"

/* Private defines -----------------------------------------------------------*/

// The following should be the minimal CircleOS version needed by your 
// application.
#define NEEDEDVERSION "V 4.0"

/* Private functions ---------------------------------------------------------*/
static enum MENU_code MsgVersion(void);

/* Public variables ----------------------------------------------------------*/
const char Application_Name[8+1] = {"Pri.War."};      // Max 8 characters											  

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
	static enum CurrentDisplay screen; // Where are we?!
	
	static struct GameData gamedata; //Things the minigames need to know (e.g. are we multiplayer?)
	
	static gameRunFunction minigameArray[ROUNDLENGTH]; // Holds current 
													   // minigames.
	static int currentMinigame = 0; // Index in the array of the current 
									// pointer.
	static int score = 0; // Current total score.
	static int lives = 3; // Number of lives remaining.
	static int amServer = 0; // Am I the server (multiplayer only of course)?
	const unsigned int stageScreenTimerValue = 200; // Timer value for stage
													// screens.
	
	// Make sure the timers count up!
	TIMER_tickTimers();
		
	if (screen == display_Menu) {
		enum MenuCode menuCode = MENUHANDLER_run(); // Handle menu and get code.
	
		// If the button is pressed, the application is exited
		if (BUTTON_GetState() == BUTTON_PUSHED) {
			BUTTON_WaitForRelease();
			return MENU_Quit();
		}
		// If we have chosen a game type...
		if (menuCode != MenuCode_Nothing) {			
			// Pupulate minigame array.
			init_rand(123);
			
			gameRunFunction* theMinigames;
			if (menuCode == MenuCode_SinglePlayer) {
				theMinigames = minigamesSinglePlayer;
				gamedata.mode = Game_SinglePlayer;
			}
			else if (menuCode == MenuCode_TwoPlayerCoOp) {
				theMinigames = minigamesCoOp;
				gamedata.mode = Game_CoOp;
			}
			else {
				theMinigames = minigamesVs;
				gamedata.mode = Game_Vs;
			}
			
			int i = 0;
			for (i = 0; i < ROUNDLENGTH; i++)
				minigameArray[i] = theMinigames[rand_cmwc() % ROUNDLENGTH];
			
			// If a multiplayer game was started, set up the multiplayer.
			if (menuCode != MenuCode_SinglePlayer) {
				// TODO: Multiplayer init code.
				DRAW_DisplayStringWithMode(0,
										   0,
										   "Multiplayah!",
										   ALL_SCREEN, 0, 1);
			}
			
			// Change from menu mode to game mode.
			screen = display_StageStart;
		}
		
	} else if (screen == display_StageStart) {
		if (!TIMER_isEnabled(0)) {
			TIMER_initTimer(0, stageScreenTimerValue);
			GAMEDRAW_stageN(currentMinigame + 1);
		}
		
		if (TIMER_checkTimer(0)) {
			TIMER_disableTimer(0);
			screen = display_Game;
		}
	} else if (screen == display_Game) {
		// Run the current game and get its status.
		
		minigameArray[currentMinigame](&gamedata);
			
		// If the game has finished...
		if (gamedata.code != gameStatus_InProgress) {
			currentMinigame++;
			
			if (gamedata.code == gameStatus_Success) {
				score += gamedata.score;
				screen = display_StageSuccess;
			} else {
				lives--;
				screen = display_StageFail;
			}
			
			gamedata.code = gameStatus_InProgress;
			gamedata.score = 0;
		}
	} else if (screen == display_StageSuccess) {
		if (!TIMER_isEnabled(0)) {
			TIMER_initTimer(0, stageScreenTimerValue);
			GAMEDRAW_stageSuccess();
		}
		
		if (TIMER_checkTimer(0)) {
			TIMER_disableTimer(0);
			screen = display_StageStart;
		}
		
		// Check if we need to end.
			if (lives == 0 || currentMinigame == ROUNDLENGTH)
				screen = display_RoundFinish;
	} else if (screen == display_StageFail) {
		if (!TIMER_isEnabled(0)) {
			TIMER_initTimer(0, stageScreenTimerValue);
			GAMEDRAW_stageFail();
		}
		
		if (TIMER_checkTimer(0)) {
			TIMER_disableTimer(0);
			screen = display_StageStart;
		}
		
		// Check if we need to end.
			if (lives == 0 || currentMinigame == ROUNDLENGTH)
				screen = display_RoundFinish;
	} else if (screen == display_RoundFinish) {
		static bool screenDrawn = 0;
		
		if (!screenDrawn) {
			GAMEDRAW_roundFinished(score, lives);
			screenDrawn = 1;
		}
		
		if (TOUCH_clickEvent().type == TouchType_Depressed) {
			screen = display_Menu;
			gamedata.mode = Game_None;
			lives = 3;
			score = 0;
			currentMinigame = 0;
		}
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