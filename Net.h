#ifndef NET_H_GUARD

#define NET_H_GUARD


#define TX_BUFFER_LEN 128
#define RX_BUFFER_LEN 128
#define PACKET_MAX_SIZE 30

//Packet handler flags
#define RX_FLAG_BADHEADER 1
#define RX_FLAG_OVERFLOW 2
//#include "circle_api.h"

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "circle_api.h"
#include "cbuffer.h"

#define TICK_PACKET_RX 1
#define TICK_DATA_TX 2


typedef void (*net_rx_callback)(u8 packet_type, u8 packet_data, u8 flags);

typedef struct _Packet {
	u8 type;
	u8 data[PACKET_MAX_SIZE]; //null terminated string!
} Packet;


void NetSetup(void);
void NetTick(void);
int TransmitBytes(struct Packet * packet);



#endif