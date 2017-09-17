#include "typedef.h"
#include "UART\UART.h"
#include "Debug_MSG\Debug_Msg.h"
#include "ADC\ADC.h"
#include "ANT/ANT_Framer.h"
#include "ANT/ANT_Message_Queue.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include <util/delay.h> 

UART_DEFINE(extern)
#define UART_ANT	UART2

//Inicjalizacja modulu ANT
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
	
   //Inicjalizaca UART
	USART_Init(IN 57600, IN UART_ANT);
	USART_Init(IN 57600, IN UART3);
	
   //Inicjalizacja ADC
	ADC_Init();	
	
	sei();
	
	DMsgMessageNewPage(IN 18, IN (unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN (unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN (unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN (unsigned char*)"");
	
   //Inicjalizacja ANT
	Status = ANT_Init();
	
	DMsgMessageNewLine(IN 2, IN (unsigned char*)"1");

   //Inicjalizacja profilu ANT+ zwiazanego z temperatura
	Status = TemperatureSensorInit();
	
	DMsgMessageNewLine(IN 2, IN (unsigned char*)"2");
			
	while(1)
   {
      //Sprawdzenie czy sa nowe wiadomosci od modulu ANT
		Status = ANT_Framer_GetMessage(&AntMessage);
		if (STATUS_SUCCESS == Status)
		{
         //Przeprocesowanie odebranej wiadomosci
			Status = ANT_Mesg_Q_ProcessPayload(&AntMessage);

         //Jesli cos poszlo nie tak, wyswietlona zostaje odebrana wiadomosc
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