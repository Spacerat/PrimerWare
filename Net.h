#ifndef NET_H_GUARD
#define NET_H_GUARD

#include "circle_api.h"

#define TX_BUFFER_LEN 128
#define RX_BUFFER_LEN 128
#define PACKET_MAX_SIZE 30

//Packet handler flags
#define RX_FLAG_BADHEADER 1
#define RX_FLAG_OVERFLOW 2
#define RX_FLAG_RECEIVED 4

//Hardware settings
#define SPIx SPI1
#define USARTx USART1


//#def USE_SPI //Uncomment to use SPI instead of IR
#define USE_IR


#define TICK_PACKET_RX 1
#define TICK_DATA_TX 2




typedef struct _Packet {
	u8 type;
	u8 data[PACKET_MAX_SIZE]; //null terminated string!
} Packet;


void NET_Setup(void);
u8 NET_NetTick(void);
int NET_TransmitBytes(Packet * packet);
u8 NET_GetPacketData(u8 * type, u8 * buffer);


#endif