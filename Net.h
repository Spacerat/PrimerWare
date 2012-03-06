#ifndef NET_H_GUARD
#define NET_H_GUARD

//#define USE_SPI //Uncomment to use SPI
#define USE_SERIAL //Uncomment to use USART serial
//#define USE_IR //Uncomment to use IR comms

#if defined(USE_SERIAL) || defined(USE_IR)
	#define USE_USART
#endif

#include "misc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "Packets.h"

#if defined(USE_SPI)
	#include "stm32f10x_spi.h"
#elif defined(USE_USART)
	#include "stm32f10x_usart.h"
#endif

#include "stm32f10x_rcc.h"
#include "circle_api.h"

#define TX_BUFFER_LEN 128
#define RX_BUFFER_LEN 128
//This is the size of the DATA component of the packets.
//The actual size in bytes is 2 bytes larger. one for 0xff at the start, another for the ID byte.
#define PACKET_MAX_SIZE 30 
#define PACKET_TOTAL_SIZE PACKET_MAX_SIZE + 2


//Packet handler flags
#define NETTICK_FLAG_BADHEADER 1
#define NETTICK_FLAG_OVERFLOW 2
#define NETTICK_FLAG_RX 4
#define NETTICK_FLAG_TX 8

//Hardware settings
#if defined(USE_SPI)
	#define SPIx SPI1
	#define GPIOx GPIOA
#elif defined(USE_IR)
	#define USARTx USART1
	#define GPIOx GPIOA
	#define USARTx_CLK RCC_APB2Periph_USART1
	#define GPIO_LEDS GPIOD
	#define GPIO_CLK RCC_APB2Periph_GPIOD 
	#define NET_TxPin GPIO_Pin_9
	#define NET_RxPin GPIO_Pin_10
	#define NET_PPPin GPIO_Pin_8
	#define NET_BAUD_RATE 115200
	
#elif defined(USE_SERIAL)
	#define USARTx USART2
	#define USARTx_CLK RCC_APB1Periph_USART2
	#define GPIOx GPIOA
	#define GPIO_CLK RCC_APB2Periph_GPIOA
	#define NET_TxPin GPIO_Pin_2
	#define NET_RxPin GPIO_Pin_3
	#define NET_BAUD_RATE 6400
	
#endif

#define TICK_PACKET_RX 1
#define TICK_DATA_TX 2


struct Packet
{
	u8 type;
	u8 data[30]; //null terminated string!
};


void NET_Setup(void);
void NET_RCC_Configuration(void);
u8 NET_Tick(void);
u8 NET_GetFlags();
int NET_TransmitPacket(struct Packet * packet);
u8 NET_GetPacketData(u8 * buffer);
u8 NET_GetPacketType( void );
int NET_TransmitStringPacket(u8 type, char * string);
void NET_enableTransmission( bool Enabled );


#endif
