
#include "Net.h"


#include "cbuffer.h"
#include <string.h>


#define BAUD_RATE 115200

#define PACKET_BEGIN 0xFF

#define USE_IR//This needs to be declared again in here for some reason.

void assert_failed(u8* file, u32 line) {}  // Required by libraries.

tHandler OldHandler; // I have no idea what this is for

CircularBuffer TXbuffer;           //Stores all data to be transmitted
bool TX_enable;

u8 RXbuffer[PACKET_TOTAL_SIZE];  //Stores all received packet data
u8 RXbufferIndex = 0;              //Counts data in the RX buffer
static u8 lastpacket_type;         //This is set when the last packet is received
static u8 net_flags = 0;

#define send_delay 12;
static u16 send_countdown = send_delay;

static bool clear_RX_buffer_at_next_tick = TRUE;

#ifdef USE_SPI //To facilitate switching between communication protocals... Yay macros!

#define GetFlagStatus(flag) SPI_I2S_GetFlagStatus(SPIx, flag)
#define SendData(data) SPI_I2S_SendData(SPIx, data)
#define ReceiveData() SPI_I2S_ReceiveData(SPIx)
#define FLAG_TXE SPI_I2S_FLAG_TXE
#define FLAG_RXNE SPI_I2S_FLAG_RXNE

#endif
#ifdef USE_IR
#warning IR enabled
#define GetFlagStatus(flag) USART_GetFlagStatus(USARTx, flag)
#define SendData(data) USART_SendData(USARTx, data)
#define ReceiveData() USART_ReceiveData(USARTx)
#define FLAG_TXE USART_FLAG_TXE
#define FLAG_RXNE USART_FLAG_RXNE

#endif

/* ----------------- Configuration things ---------------- */

/* Configure GPIO
Taken from SPI CRC Example */
__attribute__((section(".rodata"))) void GPIO_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_SPI
	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef USE_IR
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
#endif
}


/* Configure SPI or IrDA interface.
Taken from SPI CRC and IrDA examples. */
__attribute__((section(".rodata"))) void Net_Configuration(void)
{
#ifdef USE_SPI
	SPI_InitTypeDef  SPI_InitStructure;

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
	SPI_CalculateCRC(SPIx, ENABLE);
	/* Enable SPI2 CRC calculation */
	//SPI_CalculateCRC(SPI2, ENABLE);

	/* Enable SPI1 */
	SPI_Cmd(SPIx, ENABLE);
	/* Enable SPI2 */
	//SPI_Cmd(SPI2, ENABLE);
#endif
#ifdef USE_IR
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

#endif
}


/* Configure Clock
Taken from SPI CRC Example */
__attribute__((section(".rodata"))) void NET_RCC_Configuration(void)
{
	/* PCLK2 = HCLK/2 */
	//RCC_PCLK2Config(RCC_HCLK_Div2);
#ifdef USE_SPI
	/* Enable peripheral clocks --------------------------------------------------*/
	/* GPIOA, GPIOB and SPI1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	RCC_APB2Periph_SPI1, ENABLE);
#endif
#ifdef USE_IR

    //Enable clock
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOD | 
        RCC_APB2Periph_AFIO | 
        RCC_APB2Periph_USART1, ENABLE);

#endif
	/* SPI2 Periph clock enable */
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/* Call this once to set up the networking code.
Call it twice to do absolutely nothing extra */
__attribute__((section(".rodata"))) void NET_Setup(void)
{
	static bool issetup = FALSE;
	if (issetup == FALSE)
	{
		cbInit(&TXbuffer, TX_BUFFER_LEN);
		
		GPIO_Configuration();
		Net_Configuration();
		issetup = TRUE;
	}
}


/* ----------------------- Functions that do things ------------------------ */


/* Send a packet struct to the IR library for transmission.

Creates a packet from the given data and data length, then adds this to the 
transmission queue.

Returns 1 if there is no null character in the packet data (overflow).
Returns -1 if tx_enable is false.
*/
__attribute__((section(".rodata"))) int NET_TransmitPacket(struct Packet * packet)
{
	if (TX_enable == FALSE) return -1;
	cbWrite(&TXbuffer, PACKET_BEGIN);
	cbWrite(&TXbuffer, packet->type);

	int k;
	for (k = 0; k < PACKET_MAX_SIZE; k++) {
		cbWrite(&TXbuffer, (buff_t)packet->data[k]);
		if (packet->data[k] == '\0') {
			return 0;
		}
	}
	return 1;
	
}

/* Send a string to the IR library for transmission.

Creates a packet from the given data and data length, then adds this to the 
transmission queue.

Returns 1 if there is no null character in the packet data (overflow).
Returns -1 if tx_enable is false.
*/
__attribute__((section(".rodata"))) int NET_TransmitStringPacket(u8 type, char * string)
{
	if (TX_enable == FALSE) return -1;
	cbWrite(&TXbuffer, PACKET_BEGIN);
	cbWrite(&TXbuffer, type);

	int k;
	for (k = 0; k < PACKET_MAX_SIZE; k++) {
		cbWrite(&TXbuffer, (buff_t)string[k]);
		if (string[k] == '\0') {
			return 0;
		}
	}
	return 1;
	
}

/* Copy the data section of the RX buffer in to another buffer.
   Set the "type" parameter to the packet ID.
   Return the length of the data.
*/
__attribute__((section(".rodata"))) u8 NET_GetPacketData(u8 * buffer) {
	strcpy(buffer, RXbuffer + 2);
	return (u8)strlen(buffer + 2);
}

/* Return just the packet ID of the last packet */
__attribute__((section(".rodata"))) u8 NET_GetPacketType( void ) {
	return lastpacket_type;
}


__attribute__((section(".rodata"))) u8 NET_GetFlags() {
	return net_flags;
}

/* Call to enable/disable the transmit functions. The transmit functions
do nothing when Enabled is false - this lets you call them in single player
mode without caring.
*/
__attribute__((section(".rodata"))) void NET_enableTransmission( bool Enabled) {
	TX_enable = Enabled;
}

//Send/Recieve IR packets.
/*
Return value may include a number of flags:
- IR_PACKET_RX: A packet has been received
- IR_TX_EMPTY: There is no more data to send
*/
__attribute__((section(".rodata"))) u8 NET_Tick(void)
{

	//If a packet was received last tick, it should have been dealt with by now.
	//discard it from memory
	if (clear_RX_buffer_at_next_tick == TRUE) {
		u8 i;
		//for (i = 0; i < 3; i++) {
		//	RXbuffer[i] = (u8)'\0';
		//}
		RXbufferIndex = 0;
		clear_RX_buffer_at_next_tick = FALSE;
		lastpacket_type = PACKET_NULL;
		net_flags = 0;
	}

	//Send any data sitting in the buffer.
	if (!cbIsEmpty(&TXbuffer) && ((GetFlagStatus(FLAG_TXE) != RESET))) {
		if (send_countdown == 0) {
			char data;
			cbRead(&TXbuffer, &data);
			SendData((u8)data);
			net_flags |= NETTICK_FLAG_TX;
			send_countdown = send_delay;
		}
		else {
			send_countdown --;
		}
	}

	//Recieve data
	if (GetFlagStatus(FLAG_RXNE) == SET) {
		u8 ReceivedData = ReceiveData();
		RXbuffer[RXbufferIndex++] = ReceivedData;
		u8 overflow;
	
		if (ReceivedData == '\0' || (overflow = (RXbufferIndex == PACKET_TOTAL_SIZE))) {
			net_flags |= NETTICK_FLAG_RX;
			//0xFF is expected as the first value
			if (RXbuffer[0] != 0xFF) {
				net_flags |= NETTICK_FLAG_BADHEADER;
			}
			if (overflow) {
				net_flags |= NETTICK_FLAG_OVERFLOW;
				RXbuffer[PACKET_MAX_SIZE + 1] = '\0';
			}
			
			lastpacket_type = RXbuffer[1];
			clear_RX_buffer_at_next_tick = TRUE;
		}
	}

}

