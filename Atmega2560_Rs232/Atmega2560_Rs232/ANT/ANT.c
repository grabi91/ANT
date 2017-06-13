/*
 * ANT.c
 *
 * Created: 2015-11-12 14:29:58
 *  Author: Lukasz
 */ 

#include "ANT.h" 

void ANT_Reset()
{
	unsigned char setup[2];
	setup[0] = 0x4a; // ID Byte
	setup[1] = 0x00; // Data Byte N (N=LENGTH)
	
	USART_Transmit(IN UART_ANT, 2, IN &setup);	
}

void ANT_OpenChannel()
{
	unsigned char setup[2];
	setup[0] = 0x4b;
	setup[1] = 0x00;
	
	USART_Transmit(IN UART_ANT, 2, IN &setup);		
}

void ANT_AssignChannel()
{
	unsigned char setup[4];	
	setup[0] = 0x42;
	setup[1] = ANT_CH_ID;    // Channel ID, 0x00 for HRM, 0x01 for custom
	setup[2] = ANT_CH_TYPE;  // CH Type
	setup[3] = ANT_NET_ID;   // Network ID
	
	USART_Transmit(IN UART_ANT, 4, IN &setup);
}

void ANT_SetChannelRF()
{
	unsigned char setup[3];
	setup[0] = 0x45;
	setup[1] = (ANT_CH_FREQ & 0xFF00) >> 8;
	setup[2] = (ANT_CH_FREQ & 0xFF);    // RF Frequency
	
	USART_Transmit(IN UART_ANT, 3, IN &setup);
}

void ANT_SetChannelPeriod()
{
	unsigned char setup[3];
	setup[0] = 0x43;
	setup[1] = (ANT_CH_PER & 0xFF00) >> 8;
	setup[2] = (ANT_CH_PER & 0xFF);    // RF Frequency
	
	USART_Transmit(IN UART_ANT, 3, IN &setup);
}

void ANT_SetChannelId()
{
	unsigned char setup[6];
	setup[0] = 0x51;
	setup[1] = ANT_CH_ID;      // Channel Number, 0x00 for HRM
	setup[2] = ANT_DEV_ID1;    // Device Number LSB
	setup[3] = ANT_DEV_ID2;    // Device Number MSB
	setup[4] = ANT_DEV_TYPE;   // Device Type, 0x78 for HRM
	setup[5] = ANT_TX_TYPE;
	
	USART_Transmit(IN UART_ANT, 6, IN &setup);
}

