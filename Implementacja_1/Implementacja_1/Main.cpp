extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/Debug_MSG/Debug_Msg.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/ANT/ANT_Framer.h"
}

#include "UART.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int a = 0;
	STATUS Status;
	ANT_MESSAGE_ITEM AntMessage;
	unsigned char dataByte = 0;
	USART_Init(IN 57600, IN UART0);

	DMsgMessageNewPage(IN 18, IN(unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN(unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN(unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN(unsigned char*)"");

	//USART_TransmitByteByFifo(UART0, data);
	//USART_Transmit(UART0, sizeof(data), data);

	ANT_Framer_SendMessage(&AntMessage);

	while (1)
	{
		Status = ANT_Framer_GetMessage(&AntMessage);
		if (STATUS_SUCCESS == Status)
		{
			printf("%x\n", dataByte);
			ANT_Framer_SendMessage(&AntMessage);
		}

	}

	USART_Close(IN UART0);

	return 0;
}