#ifndef NET_H_GUARD
#define NET_H_GUARD

//#def USE_SPI //Uncomment to use SPI instead of IR
#define USE_IR


#include "misc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"

#ifdef USE_SPI
	#include "stm32f10x_spi.h"
#endif
#ifdef USE_IR
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
#define SPIx SPI1
#define USARTx USART1

enum PacketTypes {
	PACKET_NULL = 0,
	/* --- Menu packets --- */
	PACKET_requestGame,
	PACKET_ACKGame,

	/* --- Game packets --- */
	PACKET_gameData
	
};


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
int NET_TransmitPacket(struct Packet * packet);
u8 NET_GetPacketData(u8 * type, u8 * buffer);
int NET_TransmitStringPacket(u8 type, char * string);

#endif