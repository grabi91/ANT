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
//#include "ANT\ANT.h"
#include "ANT/ANT_Framer.h"
#include "ANT/ANT_Message_Queue.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include <util/delay.h> 

UART_DEFINE(extern)
#define UART_ANT	UART2

STATUS ANT_Init()
{
	STATUS Status = STATUS_SUCCESS;
	unsigned char NetKey[USER_NETWORK_KEY_SIZE] = USER_NETWORK_KEY;

	Status = ANT_Mesg_Q_Init();
	
	DMsgMessageNewLine(IN 2, IN (unsigned char*)"3");

	if (Status == STATUS_SUCCESS)
	{
		Status = ANT_Framer_Mesg_ResetSystem();
		DMsgMessageNewLine(IN 2, IN (unsigned char*)"4");
	}

	Status = ANT_Framer_Mesg_SetNetworkKey(USER_NETWORK_NUM, NetKey, USER_NETWORK_KEY_SIZE);

	return Status;
}

int main(void)
{
	STATUS Status;
	ANT_MESSAGE_ITEM AntMessage;
	
   //zakazane USART_Init(IN BAUD, IN UART0);
	USART_Init(IN BAUD, IN UART1);
	USART_Init(IN 57600, IN UART_ANT);
	USART_Init(IN 57600, IN UART3);
	
	ADC_Init();	
	
	//DDRF = 0xFF; //konfiguracja portu jako wyjœcia
	
	//MCUCR |= (1<<IVSEL);
	
	sei();
	
	DMsgMessageNewPage(IN 18, IN (unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN (unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN (unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN (unsigned char*)"");
	
	Status = ANT_Init();
	
	DMsgMessageNewLine(IN 2, IN (unsigned char*)"1");

	Status = TemperatureSensorInit();
	
	DMsgMessageNewLine(IN 2, IN (unsigned char*)"2");
			
	while(1)
    {
		Status = ANT_Framer_GetMessage(&AntMessage);
		if (STATUS_SUCCESS == Status)
		{
			Status = ANT_Mesg_Q_ProcessPayload(&AntMessage);

			if (Status != STATUS_SUCCESS)
			{
				unsigned char temp[4];
				sprintf(temp, "0x%02x ", AntMessage.AntMessage.MessageID);
				DMsgMessage(sizeof(temp), temp);
				
				for (int i = 0; i < AntMessage.Size; i++)
				{
					unsigned char temp[4];
					sprintf(temp, "0x%02x ", AntMessage.AntMessage.Data[i]);
					DMsgMessage(sizeof(temp), temp);
				}
			}
		}
    }
}