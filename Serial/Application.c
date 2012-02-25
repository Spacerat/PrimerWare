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
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "circle_api.h"
/* Private defines -----------------------------------------------------------*/


void assert_failed(u8* file, u32 line) {}  // Required by libraries. WE don't bother to handle it.

/* Private variables ---------------------------------------------------------*/

tHandler OldHandler;
u8 OldPLLFreq;

SPI_InitTypeDef  SPI_InitStructure;
/* Private functions ---------------------------------------------------------*/


/* Public variables ----------------------------------------------------------*/

const char Application_Name[8+1] = {"EchoSPI"};  // max 8 characters for application name

enum MENU_code Application_Handler ( void );

void GPIO_Configuration(void) ;
void RCC_Configuration(void);

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void RCC_Configuration(void)
{
	/* PCLK2 = HCLK/2 */
	//RCC_PCLK2Config(RCC_HCLK_Div2);
	
	/* Enable peripheral clocks --------------------------------------------------*/
	/* GPIOA, GPIOB and SPI1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	RCC_APB2Periph_SPI1, ENABLE);
	
	/* SPI2 Periph clock enable */
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

void SPI_Configuration(void)
{
	/* SPI1 configuration ------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* SPI2 configuration ------------------------------------------------------*/
	//SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	//SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI1 CRC calculation */
	SPI_CalculateCRC(SPI1, ENABLE);
	/* Enable SPI2 CRC calculation */
	//SPI_CalculateCRC(SPI2, ENABLE);

	/* Enable SPI1 */
	SPI_Cmd(SPI1, ENABLE);
	/* Enable SPI2 */
	//SPI_Cmd(SPI2, ENABLE);
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
char send_string[]="Hello SPI! ";
int send_offset = 0;
const char greet_string[]=" * SPI R/W * ";



enum MENU_code Application_Ini ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	OldPLLFreq = UTIL_GetPll ();  // save for restore on exit
	
	//Enable clock
	/*
	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_GPIOD |
	RCC_APB2Periph_AFIO |
	RCC_APB2Periph_USART1, ENABLE);
	*/
	RCC_Configuration();
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
		GPIO_Configuration();
		SPI_Configuration();
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
		&& ((SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != RESET))) {
			if (delay ==0) {
				u8 d = (u8)send_string[send_offset];
				char c = (char) d;
				SPI_I2S_SendData(SPI1, d);
				send_offset++;
				delay = send_delay;
				} else {
				delay = delay -1;
			}
			} else if ( JOYSTICK_GetState() == JOYSTICK_RELEASED) {
			handler_state = STATE_idle;
		}
		try_receive:
		if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET) {
			u8 ReceivedData = SPI_I2S_ReceiveData(SPI1);
			char buf[2];
			buf[0]=(char) ReceivedData;
			buf[1]='\0';
			DRAW_DisplayString(hpos,vpos,buf,2);
			hpos = hpos + CHAR_WIDTH + 1;
			if ((hpos + CHAR_WIDTH) >= 127) {
				hpos = 0;
				vpos = vpos - CHAR_HEIGHT+1;
				if (vpos < 0) {
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