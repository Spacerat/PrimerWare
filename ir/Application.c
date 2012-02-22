/************************* (C) COPYRIGHT 2007 RAISONANCE **********************
* File Name          :  Echo.c
* Author             :
* Date First Issued  :
* Description        :  Circle_App CircleOS application template.
* Revision           :
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "circle_api.h"

/* Private defines -----------------------------------------------------------*/


void assert_failed(u8* file, u32 line) {}  // Required by libraries. WE don't bother to handle it.

/* Private variables ---------------------------------------------------------*/

tHandler OldHandler;
u8 OldPLLFreq;


/* Private functions ---------------------------------------------------------*/


/* Public variables ----------------------------------------------------------*/

const char Application_Name[8+1] = {"EchoIrDA"};  // max 8 characters for application name

enum MENU_code Application_Handler ( void );

void IRDA_USART_Configuration( void ) ;
void IRDA_GPIO_Configuration(void) ; 



void IRDA_GPIO_Configuration(void)
    {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART1 Tx (PA.9)as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx PA.10 as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure GPIO_D Pin 8 = CS Irda */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    }



void IRDA_USART_Configuration( void ) 
    {
    USART_InitTypeDef USART_InitStructure;

    /* USART3 configuration ------------------------------------------------------*/
    /* USART3 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


    ////* Configure the USART3 */
    /* Configure the USART1 */
    USART_Init(USART1, &USART_InitStructure);
    ////* Enable the USART3 */
    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);

    ////* Set the USART3 prescaler */
    /* Set the USART1 prescaler */
    USART_SetPrescaler(USART1, 0x1);
    ////* Configure the USART3 IrDA mode */
    /* Configure the USART1 IrDA mode */
    USART_IrDAConfig(USART1, USART_IrDAMode_Normal);

    ////* Enable the USART3 IrDA mode */
    /* Enable the USART1 IrDA mode */
    USART_IrDACmd(USART1, ENABLE);

    // CS Irda  = 1
    GPIO_ResetBits(GPIOD, GPIO_Pin_8);
    }

/*******************************************************************************
* Function Name  : Application_Ini
* Description    : Initialization function of Circle_App. This function will
*                  be called only once by CircleOS.
* Input          : None
* Return         : MENU_CONTINUE_COMMAND
*******************************************************************************/
enum STATE {STATE_init=0, STATE_idle=1, STATE_sending=2, STATE_exiting=3} ;
handler_state = STATE_init; 
enum { 
    init_vpos = ((CHAR_HEIGHT+1) * 7),  //the enum trick for naming constants
    send_delay = 0
};
int vpos = init_vpos;
int hpos = 0;
int delay = send_delay;              // seems to work OK with no delay
char send_string[]="Hello IRDA!";
int send_offset = 0;
const char greet_string[]=" * IrDA R/W * ";

enum MENU_code Application_Ini ( void )
    {
    NVIC_InitTypeDef NVIC_InitStructure;

    OldPLLFreq = UTIL_GetPll ();  // save for restore on exit

    //Enable clock
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOD | 
        RCC_APB2Periph_AFIO | 
        RCC_APB2Periph_USART1, ENABLE);
    UTIL_SetPll ( SPEED_VERY_HIGH );

    // Re-initialise our variables in case we are restarting
    handler_state = STATE_init; 
    int vpos = init_vpos;
    int hpos = 0;
    int delay = send_delay;              // seems to work OK with no delay
    int send_offset = 0;

    LCD_FillRect ( 0,0,128,128, RGB_WHITE );    return MENU_CONTINUE_COMMAND;
    }

/*******************************************************************************
* Function Name  : Application_Handler
* Description    : Management of the Circle_App.
*
* Input          : None
* Return         : MENU_CONTINUE
*******************************************************************************/


enum MENU_code Application_Handler ( void ) {
   switch (handler_state)
       {
       case STATE_init:

            DRAW_DisplayString(5,70,greet_string,sizeof(greet_string));
            IRDA_GPIO_Configuration();
            IRDA_USART_Configuration();
            handler_state = STATE_idle;
            break;

       case STATE_idle:
          if ( JOYSTICK_GetState() == JOYSTICK_DOWN ) {
              handler_state = STATE_exiting;
          } else if (JOYSTICK_GetState() == JOYSTICK_UP){
              handler_state = STATE_sending;
              delay = 0;
              send_offset =0;
          } 
          goto try_receive;

      case STATE_sending:
          if ( (send_string[send_offset] != '\0') 
                && ((USART_GetFlagStatus(USART1, USART_FLAG_TXE) != RESET))) {
              if (delay ==0) {
                  USART_SendData(USART1,(u8)send_string[send_offset]);
                  send_offset = send_offset + 1;
                  delay = send_delay;
              } else {
                  delay = delay -1;
              }
              } else if ( JOYSTICK_GetState() == JOYSTICK_RELEASED) {
                  handler_state = STATE_idle;
              }
       try_receive:  
              if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
                  u8 ReceivedData = USART_ReceiveData(USART1);
                  char buf[2];
                  buf[0]=(char) ReceivedData;
                  buf[1]='\0';
                  DRAW_DisplayString(hpos,vpos,buf,2);
                  hpos = hpos + CHAR_WIDTH + 1;
                  if ((hpos + CHAR_WIDTH) >= 127) {
                      hpos = 0;
                      vpos = vpos - CHAR_HEIGHT+1;
                      if (vpos <0) {
                          vpos = init_vpos;
                      }
                  }
              }
              break;
          case STATE_exiting:
              if ( JOYSTICK_GetState() == JOYSTICK_RELEASED) {
                  BUTTON_SetMode  ( BUTTON_ONOFF_FORMAIN );
                  UTIL_SetPll (OldPLLFreq);
                  MENU_Quit();
  /* We Quit rather than LEAVE so all the hardware gets properly reset */
  /*              return MENU_LEAVE;                                  */
              }
              break;
      }
      return MENU_CONTINUE;
}  
