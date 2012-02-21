/******************************************************************************
*
* File Name          :  Application.c
* Description        :  Main application source file.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle_api.h"
#include "MenuDraw.h"

/* Private defines -----------------------------------------------------------*/

// The following should be the minimal CircleOS version needed by your 
// application.
#define NEEDEDVERSION "V 4.0"

/* Private functions ---------------------------------------------------------*/
static enum MENU_code MsgVersion(void);

/* Public variables ----------------------------------------------------------*/
const char Application_Name[8+1] = {"My App"};      // Max 8 characters


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

	// Draw the menu.
	drawMenu();
    
    // Your can set the frequency of the call by the system.
    MENU_SetAppliDivider(1000);

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

    // TODO: Write your application handling here.
#if 0	
    static int cnt = 0;
    DRAW_SetCharMagniCoeff(2);
    DRAW_SetTextColor(RGB_BLUE);
    DRAW_SetBGndColor(RGB_WHITE);
    if ((cnt % 2) == 0)
        {
        DRAW_DisplayStringWithMode( 0, 90, "Hello", ALL_SCREEN, INVERTED_TEXT, CENTER);
        DRAW_DisplayStringWithMode( 0, 60, "world !", ALL_SCREEN, INVERTED_TEXT, CENTER);
        }
    else
        {
        DRAW_DisplayStringWithMode( 0, 90, "", ALL_SCREEN, INVERTED_TEXT, CENTER);
        DRAW_DisplayStringWithMode( 0, 60, "", ALL_SCREEN, INVERTED_TEXT, CENTER);
        }
    DRAW_SetCharMagniCoeff(1);
    DRAW_SetDefaultColor();
    cnt++;
#endif

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
