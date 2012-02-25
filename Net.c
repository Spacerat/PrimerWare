
#include "Net.h"


#define BAUD_RATE 115200

#define PACKET_BEGIN 0xFF


void assert_failed(u8* file, u32 line) {}  // Required by libraries. WE don't bother to handle it.

tHandler OldHandler; // I have no idea what this is for

CircularBuffer TXbuffer;
u8 RXbuffer[PACKET_MAX_SIZE + 1];
u8 RXbufferIndex = 0;

#define SPIx SPI1

//To facilitate switching between communication protocals if needed...
#define GetFlagStatus(flag) SPI_I2S_GetFlagStatus(SPIx, flag)
#define SendData(data) SPI_I2S_SendData(SPIx, data)
#define ReceiveData() SPI_I2S_ReceiveData(SPIx)
#define FLAG_TXE SPI_I2S_FLAG_TXE
#define FLAG_RXNE SPI_I2S_FLAG_RXNE

/*Configuration things */
#if 1
//Configure GPIO
/*
Taken from SPI CRC Example
*/
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


//Configure SPI
/*
Taken from SPI CRC Example
*/
void SPI_Configuration(void)
{
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
}

//Configure Clock
/*
Taken from SPI CRC Example
*/
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

void NetSetup(void)
{
	cbInit(&TXbuffer, TX_BUFFER_LEN);
	cbInit(&RXbuffer, RX_BUFFER_LEN);
	GPIO_Configuration();
	RCC_Configuration();
	SPI_Configuration();
}
#endif  


//Send a string to the IR library for transmission.
/*
Creates a packet from the given data and data length, then adds this to the 
transmission queue.

Returns 1 if there is no null character in the packet data (overflow).
*/
int TransmitBytes(struct Packet * packet)
{
	cbWrite(&TXbuffer, PACKET_BEGIN);
	cbWrite(&TXbuffer, packet->type);

	int k;
	for (k = 0; k < PACKET_MAX_SIZE; k++) {
		cbWrite((buff_t)packet->data[k]);
		if (packet->data[k] == '\0') {
			return 0;
		}
	}
	return 1;
	
}

//Send/Recieve IR packets.
/*
Return value may include a number of flags:
- IR_PACKET_RX: A packet has been received
- IR_TX_EMPTY: There is no more data to send
*/
char NetTick(net_rx_callback callback)
{
	//Send any data sitting in the buffer.
	if (!cbIsEmpty(TXbuffer) && ((GetFlagStatus(FLAG_TXE) != RESET))) {
		char data;
		cbRead(TXbuffer, &data);
		SendData((u8)data);
	}
	
	//Recieve data
	if (GetFlagStatus(( SPI_I2S_FLAG_RXNE) == SET) {
		u8 ReceivedData = ReceiveData();
		RXbuffer[RXbufferIndex++] = ReceivedData;
		u8 overflow;
		if (ReceivedData == '\0' || (overflow = (RXbufferIndex == PACKET_MAX_LENGTH + 2))) {
			u8 flags = 0;
			//0xFF is expected as the first value
			if (RXbuffer[0] != 0xFF) {
				flags |= RX_FLAG_BADHEADER;
			}
			if (overflow) {
				flags |= RX_FLAG_OVERFLOW;
			}
			
			u8 type = RXbuffer[1];
			u8 data[] = RXbuffer + 2;
			callback(type, data, flags);
		}
	}

}