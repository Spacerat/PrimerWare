/******************************************************************************
*
* File Name          :  Application.c
* Description        :  Main application source file.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/



#include "stm32f10x.h"
#include "Application.h"
#include "MenuHandler.h"
#include "RandNo.h"
#include "GameDraw.h"
#include "Timer.h"
#include "Net.h"
#include "GameHandler.h"
#include "GamesList.h"
#include "Touchscreen.h"
#include "circle_api.h"


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
	
    MENU_SetAppliDivider(10);

	NET_RCC_Configuration();

	UTIL_SetPll(SPEED_VERY_HIGH);
    return MENU_CONTINUE_COMMAND;
    }

static gameRunFunction minigameArray[ROUNDLENGTH]; // Holds current minigames
static u8 numMinigames;
/* 
This function just fills the global minigameArray with the contents of the supplied
array of game functions, and sets numMinigames to the num argument.
*/
void startMinigames(gameRunFunction* theMinigames, const u8 num) {
	u8 i = 0;
	numMinigames = num;
	for (i = 0; i < ROUNDLENGTH; i++)
		minigameArray[i] = theMinigames[i % num];
}

#define PACKETDATA_REQUESTGAME_COOP "coop"
#define PACKETDATA_REQUESTGAME_VERSUS "versus"

void Transmit_RequestGame(bool isCoOp) {
	if (isCoOp == TRUE)
		NET_TransmitStringPacket(PACKET_requestGame, PACKETDATA_REQUESTGAME_COOP);
	else
		NET_TransmitStringPacket(PACKET_requestGame, PACKETDATA_REQUESTGAME_VERSUS);
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
	
	static struct GameData gamedata; // Things the minigames need to know
									 // (e.g. are we multiplayer?)
	

	static int currentMinigame = 0; // Index in the array of the current 
									// pointer.
	static int gamesPlayed = 0;
	
	static int score = 0; // Current total score.
	static int lives = 3; // Number of lives remaining.
	static int amServer = 0; // Am I the server (multiplayer only of course)?
	const unsigned int stageScreenTimerValue = 200; // Timer value for stage
													// screens.
	static bool gameRequested = FALSE; //Did we just request a game?
	
	NET_Setup(); //Calling this multiple times does nothing.
	
	// Make sure the timers count up!
	TIMER_tickTimers();
	NET_Tick();
	
	if (screen == display_Menu) {
		enum MenuCode menuCode = MENUHANDLER_run(); // Handle menu and get code.
	
		//Recieve data - check if someone wants to start a game with us.
		if (NET_GetFlags() & NETTICK_FLAG_RX) {
			u8 type;
			u8 buffer[PACKET_MAX_SIZE];
			NET_GetPacketData(buffer);
			switch (NET_GetPacketType()) {
				case PACKET_requestGame:
					//Other player has requested a game.
					//Send the data back as an ACK
					NET_TransmitStringPacket(PACKET_ACKGame, buffer);
					break;
				case PACKET_ACKGame:
					if (gameRequested == FALSE) {
						//Master sent slave an ACK, slave now begins.
						gamedata.isHost = FALSE;
						screen = display_StageStart;
					}

					else {
						//Slave sent master an ACK, master should begin in roughly 8 ticks...
						//And also complete the handshake.
						gamedata.isHost = TRUE;
						NET_TransmitStringPacket(PACKET_ACKGame, buffer);
						TIMER_initTimer(0, 8);
					}
					
					if (strcmp(buffer, PACKETDATA_REQUESTGAME_COOP) == 0) {
						startMinigames(minigamesCoOp, NUMCOOPGAMES);
						gamedata.mode = Game_CoOp;
					}
					else if (strcmp(buffer, PACKETDATA_REQUESTGAME_VERSUS) == 0) {
						startMinigames(minigamesVs, NUMVSGAMES);
						gamedata.mode = Game_Vs;
					}
					break;
			}
		}
	
		if (TIMER_checkTimer(0) && TIMER_isEnabled(0) && gameRequested == TRUE) {
			/*
			The master has waited for the roughly 8 ticks it should take to send 
			an ACK to the slave, so begins the game. Thus they (theoretically) 
			start at the same time.
			*/
			TIMER_disableTimer(0);
			screen = display_StageStart;
		}
	
		// If the button is pressed, the application is exited
		if (BUTTON_GetState() == BUTTON_PUSHED) {
			BUTTON_WaitForRelease();
			return MENU_Quit();
		}
		// If we have chosen a game type...
		if (menuCode != MenuCode_Nothing) {			
			// Populate minigame array.
			init_rand(123);
			
			if (menuCode == MenuCode_SinglePlayer) {
				//Jump straight in if single player is selected
				startMinigames(minigamesSinglePlayer, NUMSINGLEGAMES);
				gamedata.mode = Game_SinglePlayer;
				gamedata.isHost = TRUE;
				screen = display_StageStart;
			
			}
			else if (menuCode == MenuCode_TwoPlayerCoOp) {
				//If multiplayer, send a request and wait for an ACK
				Transmit_RequestGame(TRUE);
				gameRequested = TRUE;

			}
			else {
				Transmit_RequestGame(FALSE);
				gameRequested = TRUE;
				//theMinigames = minigamesVs;
				//gamedata.mode = Game_Vs;
			}
			

			
			// Change from menu mode to game mode.
			
		}
		
	} else if (screen == display_StageStart) {
		if (!TIMER_isEnabled(0)) {
			TIMER_initTimer(0, stageScreenTimerValue);
			GAMEDRAW_stageN(currentMinigame + 1);
		}
		
		if (TIMER_checkTimer(0)) {
			TIMER_disableTimer(0);
			//Begin the game and allow it to run.
			screen = display_Game;
			gamedata.code = gameStatus_InProgress;
		}
	} else if (screen == display_Game) {
		// Run the current game and get its status.
	
		/*
		TODO:: IF WE ARE THE SLAVE, INTERCEPT "YOU WIN/LOSE!" PACKETS HERE
		       IF WE ARE THE MASTER, SEND THEM BASED ON THE GAME STATUS
		*/
		
		if (gamedata.code == gameStatus_InProgress) {
			minigameArray[currentMinigame](&gamedata);
		}
	
		// Slave checks for "You win/lose!" packets and overrides the 
		// game's reported status
		if (gamedata.isHost == FALSE && NET_GetFlags() & NETTICK_FLAG_RX) {
			u8 type = NET_GetPacketType();
			u8 data[PACKET_MAX_SIZE];
			switch (type) {
					gamedata.code = gameStatus_Success;
					break;
				case PACKET_gameFail:
					gamedata.code = gameStatus_Fail;
					break;
			}
		}
			
		// If the game has finished...
		if (gamedata.code != gameStatus_InProgress) {
			currentMinigame = rand_cmwc() % numMinigames;
			
			if (gamedata.code == gameStatus_Success) {
				score += gamedata.score;
				screen = display_StageSuccess;
			} else {
				lives--;
				screen = display_StageFail;
			}
			gamesPlayed ++;
			gamedata.code = gameStatus_InProgress;
			gamedata.score = 0;
		}
	} else if (screen == display_StageSuccess || screen == display_StageFail) {
		static bool screenDrawn = FALSE;
		if (!screenDrawn) {
			if (screen == display_StageSuccess)
				GAMEDRAW_stageSuccess();
			else
				GAMEDRAW_stageFail();
			screenDrawn = TRUE;
		}
		
		//Master waits a few moments until starting a new game.
		//Slave waits for the master to tell it which game to play.
		if (gamedata.isHost) {
			if(!TIMER_isEnabled(0)) {
				TIMER_initTimer(0, stageScreenTimerValue);
			}
			if (TIMER_checkTimer(0)) {
				TIMER_disableTimer(0);
				
				// Check if we need to end the round.
				if (lives == 0 || gamesPlayed == ROUNDLENGTH)
					screen = display_RoundFinish;
				else
					screen = display_StageStart;
				screenDrawn = FALSE;
			}
		}
	} else if (screen == display_RoundFinish) {
		static bool screenDrawn = FALSE;
		
		if (!screenDrawn) {
			GAMEDRAW_roundFinished(score, lives);
			screenDrawn = TRUE;
		}
		
		if (TOUCH_clickEvent().type == TouchType_Depressed) {
			MENUHANDLER_setDrawn(FALSE);
			//Reset *everything*
			screen = display_Menu;
			gamedata.mode = Game_None;
			lives = 3;
			score = 0;
			currentMinigame = 0;
			screenDrawn = FALSE;
			gameRequested = FALSE;
			gamesPlayed = 0;
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