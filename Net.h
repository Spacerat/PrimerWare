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
#define PACKET_MAX_SIZE 30

//Packet handler flags
#define RX_FLAG_BADHEADER 1
#define RX_FLAG_OVERFLOW 2

//Hardware settings
#define SPIx SPI1
#define USARTx USART1




#define TICK_PACKET_RX 1
#define TICK_DATA_TX 2


typedef void (*net_rx_callback)(u8 packet_type, u8 * packet_data, u8 flags);

typedef struct _Packet {
	u8 type;
	u8 data[PACKET_MAX_SIZE]; //null terminated string!
} Packet;


void NetSetup(void);
u8 NetTick(net_rx_callback callback);
int TransmitBytes(Packet * packet);



#endif