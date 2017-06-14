#include "ANT_Framer.h"
#include "../typedef.h"
#ifdef WIN32
#include "../../../Implementacja_1/Implementacja_1/UART.h"
#else
#include "../Rs232/Rs232.h"
#endif

STATUS ANT_Framer_GetMessage(ANT_MESSAGE_ITEM* pAntMessage)
{
	unsigned char dataByte;
	STATUS Status = STATUS_SUCCESS;
	unsigned char data[] = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };

	Status = USART_ReadByteFromFifo(UART0, OUT &dataByte);

	USART_Transmit(UART0, sizeof(data), data);
}