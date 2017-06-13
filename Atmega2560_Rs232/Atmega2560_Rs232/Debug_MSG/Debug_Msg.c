/*
 * Debug_Msg.c
 *
 * Created: 2015-09-28 18:43:18
 *  Author: Lukasz
 */ 
#include "Debug_Msg.h"
#ifdef WIN32
#include <stdio.h>
#endif

STATUS DMsgMessage(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage)
{
	#ifndef DEBUG_MSG
		return STATUS_NDEF_DMsg;	
	#endif /* DEBUG_MSG	 */
	
#ifdef WIN32
		printf(pMessage);
		return STATUS_SUCCESS;
#else
	return USART_Transmit(IN UART_DMsg, IN MessageLength, pMessage);
#endif
}

STATUS DMsgMessageNewPage(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage)
{
	#ifndef DEBUG_MSG
		return STATUS_NDEF_DMsg;
	#endif /* DEBUG_MSG	 */
	
#ifdef WIN32
		printf("\f%s", pMessage);
		return STATUS_SUCCESS;
#else
	if(STATUS_FAILURE == USART_TransmitByteByFifo( IN UART_DMsg, IN 0xC))
		return STATUS_FAILURE;
	
	return USART_Transmit(IN UART_DMsg, IN MessageLength, pMessage);
#endif
}

STATUS DMsgMessageNewLine(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage)
{
	#ifndef DEBUG_MSG
		return STATUS_NDEF_DMsg;
	#endif /* DEBUG_MSG	 */
	
#ifdef WIN32
		printf("\n%s", pMessage);
		return STATUS_SUCCESS;
#else
	if(STATUS_FAILURE == USART_TransmitByteByFifo( IN UART_DMsg, IN 0xA))
		return STATUS_FAILURE;
		
	if(STATUS_FAILURE == USART_TransmitByteByFifo( IN UART_DMsg, IN 0xD))
		return STATUS_FAILURE;	
	
	return USART_Transmit(IN UART_DMsg, IN MessageLength, pMessage);
#endif
}
