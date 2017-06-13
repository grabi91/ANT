extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/Debug_MSG/Debug_Msg.h"
}

#include "UART.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	STATUS Status;
	unsigned char data[] = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };
	unsigned char dataByte;
	USART_Init(IN 57600, IN UART0);

	DMsgMessageNewPage(IN 18, IN(unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN(unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN(unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN(unsigned char*)"");

	//USART_TransmitByteByFifo(UART0, data);
	USART_Transmit(UART0, sizeof(data), data);

	while (1)
	{
		Status = USART_ReadByteFromFifo(UART0, OUT &dataByte);
		if (STATUS_SUCCESS == Status)
		{
			printf("%x\n", dataByte);
		}
	}

	USART_Close(IN UART0);

	return 0;
}