/*
 * Atmega2560_Rs232.c
 *
 * Created: 2015-09-01 18:11:58
 *  Author: Lukasz
 */ 
#include "typedef.h"
#include "Rs232\Rs232.h"
#include "Debug_MSG\Debug_Msg.h"
#include "ADC\ADC.h"
#include "ANT\ANT.h"
#include <util/delay.h> 

UART_DEFINE(extern)

int main(void)
{
   //zakazane USART_Init(IN BAUD, IN UART0);
	USART_Init(IN BAUD, IN UART1);
	USART_Init(IN 1200, IN UART_ANT);
	USART_Init(IN BAUD, IN UART3);
	
	ADC_Init();	
	
	//DDRF = 0xFF; //konfiguracja portu jako wyjœcia
	
	unsigned char data, ANT_DataResponse[2] = {0xFF, 0xFF};
	STATUS Status;
	
	//MCUCR |= (1<<IVSEL);
	
	sei();
	
	DMsgMessageNewPage(IN 18, IN (unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN (unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN (unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN (unsigned char*)"");
			
	while(1)
    {
		Status = USART_ReadByteFromFifo(IN UART_ANT, OUT &data);
		if (STATUS_SUCCESS == Status)
		{
			ANT_DataResponse[0] = data;
			
			if(ANT_DataResponse[0] != ANT_DataResponse[1])
			{
				char message[20];
				sprintf(message, "Odpowiedz = 0x%x", data);
				DMsgMessageNewLine(IN 20, IN message);	
				
				ANT_DataResponse[1] = ANT_DataResponse[0];
			}
		}
		
		Status = USART_ReadByteFromFifo(IN UART3, OUT &data);
		if (STATUS_SUCCESS == Status)
		{
			if(data == 180)
			{
				PORTF = 0xFF;	
			}
			if(data == 170)
			{
				PORTF = 0x00;	
			}
			if(data == 150)
			{
				ADC_RESPONSE adc_value;
				char message[20];
				ADC_Read(IN 8, OUT &adc_value);
								
				sprintf(message, "Wartosc = %d", adc_value);
				
				DMsgMessageNewLine(IN 20, IN message);
			}
			//USART_TransmitByteByFifo(IN UART3, IN data);
			if(data == 160)
			{
				ANT_Reset();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"160");	
			}
			if(data == 161)
			{
				ANT_AssignChannel();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"161");	
			}
			if(data == 162)
			{
				ANT_SetChannelRF();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"162");
			}
			if(data == 163)
			{
				ANT_SetChannelPeriod();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"163");
			}
			if(data == 164)
			{
				ANT_SetChannelId();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"164");
			}
			if(data == 165)
			{
				ANT_OpenChannel();
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"165");
			}
			if(data == 169)
			{
				ANT_Reset();
				ANT_AssignChannel();
				ANT_SetChannelRF();
				ANT_SetChannelPeriod();
				ANT_SetChannelId();
				ANT_OpenChannel();
				
				DMsgMessageNewLine(IN 3, IN (unsigned char*)"169");
				
			}
			
		}
		
// 		PORTF = 0x00;
// 		_delay_ms(100);
// 		PORTF = 0xFF;
// 		_delay_ms(100);
// 		
// 		PORTF = 0x00;
// 		_delay_ms(100);
// 		PORTF = 0xFF;
// 		_delay_ms(100);
    }
}