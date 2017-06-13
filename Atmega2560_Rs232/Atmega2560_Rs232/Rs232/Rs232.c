/*
 * Rs232.c
 *
 * Created: 2015-09-01 19:13:12
 *  Author: Lukasz
 */ 
#include "Rs232.h"

UART_DEFINE()

void PickUart(IN UART_ENUM Uart, INOUT FIFO **pFifoIn, INOUT FIFO **pFifoOut, INOUT UART_ADDRESS *UartAddress)
{
	if(pFifoIn != NULL)
	{
		switch(Uart)
		{
			case UART0:
				*pFifoIn = &UART0FifoIn;
				break;
			case UART1:
				*pFifoIn = &UART1FifoIn;
				break;
			case UART2:
				*pFifoIn = &UART2FifoIn;
				break;
			case UART3:
				*pFifoIn = &UART3FifoIn;
				break;
		}	
	}
	
	if(pFifoOut != NULL)
	{
		switch(Uart)
		{
			case UART0:
				*pFifoOut = &UART0FifoOut;
				break;
			case UART1:
				*pFifoOut = &UART1FifoOut;
				break;
			case UART2:
				*pFifoOut = &UART2FifoOut;
				break;
			case UART3:
				*pFifoOut = &UART3FifoOut;
				break;
		}
	}
	
	if(UartAddress != NULL)
	{
		UART_REGISTER_DEFINE(UartAddress, Uart)		
	}
} 

void USART_Init( IN uint32_t baud, IN UART_ENUM uart )
{
	unsigned int ubbr;
	FIFO *pFifoIn, *pFifoOut;
	UART_ADDRESS UartAddress;
	
	ubbr = (F_CPU / (16 * baud)) - 1;
	
	PickUart(IN uart, INOUT &pFifoIn, INOUT &pFifoOut, INOUT &UartAddress);	
	
	//FifoInit(OUT pFifoIn);
	FifoInit(OUT pFifoOut);
	FifoInit(OUT pFifoIn);
	/* Set baud rate */
	REGISTER_ADRESS_8(UartAddress.UBBRH) = (unsigned char)(ubbr>>8);  
	REGISTER_ADRESS_8(UartAddress.UBBRL) = (unsigned char)ubbr;
	
	/* Enable receiver and transmitter and receiver interrupt*/
 	REGISTER_ADRESS_8(UartAddress.UCSRB) = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	REGISTER_ADRESS_8(UartAddress.UCSRB) &= ~(1<<UDRIE0);
 	/* Set frame format: 8data, 1stop bit */
 	REGISTER_ADRESS_8(UartAddress.UCSRC) = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) |
 	(0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) |
 	(0 << UMSEL00);
	 
	//REGISTER_ADRESS_8(UartAddress.UCSRA) = (1<<TXC0);
}

STATUS USART_WriteToFifo(IN UART_ENUM Uart, IN unsigned char data)
{
	FIFO  *pFifoIn;
	
	PickUart(IN Uart, INOUT &pFifoIn, NULL, NULL);
	
	return WriteToFifo (INOUT pFifoIn, IN data);
}

ISR (USART1_RX_vect)
{
	//przerwanie generowane po odebraniu bajtu
	USART_WriteToFifo(IN UART1, IN UDR1);
}

ISR (USART2_RX_vect)
{
	//przerwanie generowane po odebraniu bajtu
	USART_WriteToFifo(IN UART2, IN UDR2);
}

ISR (USART3_RX_vect)
{
	//przerwanie generowane po odebraniu bajtu
	USART_WriteToFifo(IN UART3, IN UDR3);
}

STATUS USART_UDRE(IN UART_ENUM Uart)
{
	UART_ADDRESS UartAddress;
	FIFO  *pFifoOut;
	unsigned char data;
	STATUS Status;
	
	PickUart(IN Uart,  NULL, INOUT &pFifoOut, INOUT &UartAddress);
	
	Status = ReadFromFifo (INOUT pFifoOut, OUT &data);	
	
	if (STATUS_SUCCESS == Status)
	{
		REGISTER_ADRESS_8(UartAddress.UDR) = data;
	}
	else
	{
		REGISTER_ADRESS_8(UartAddress.UCSRB) &= ~(1<<UDRIE3); //wy³¹cz przerwania pustego bufora nadawania
	}
	
	return Status;
}

ISR (USART1_UDRE_vect)
{
	//przerwanie generowane, gdy bufor nadawania jest ju¿ pusty,
	USART_UDRE(IN UART1);	
}

ISR (USART2_UDRE_vect)
{
	//przerwanie generowane, gdy bufor nadawania jest ju¿ pusty,
	USART_UDRE(IN UART2);
}

ISR (USART3_UDRE_vect)
{
	//przerwanie generowane, gdy bufor nadawania jest ju¿ pusty,
	USART_UDRE(IN UART3);
}

STATUS USART_ReadByteFromFifo(IN UART_ENUM Uart, OUT unsigned char *pData)
{
	FIFO  *pFifoIn;
	
	PickUart(IN Uart, INOUT &pFifoIn, NULL, NULL);
	
	return ReadFromFifo (INOUT pFifoIn, IN pData);
}

STATUS USART_TransmitByteByFifo( IN UART_ENUM Uart, IN unsigned char data)
{
	UART_ADDRESS UartAddress;
	FIFO  *pFifoOut;
	
	PickUart(IN Uart, NULL, INOUT &pFifoOut, INOUT &UartAddress);
	
	if(STATUS_SUCCESS == WriteToFifo (INOUT pFifoOut, IN data))
	{
		REGISTER_ADRESS_8(UartAddress.UCSRB) |= (1<<UDRIE3);
		
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_FAILURE;
	}
}

STATUS USART_Transmit(IN UART_ENUM Uart, IN UART_LENGTH UartLength, IN unsigned char *pUartData)
{
	int i;
	UART_ADDRESS UartAddress;
	FIFO  *pFifoOut;
	STATUS Status;
		
	PickUart(IN Uart, NULL, INOUT &pFifoOut, INOUT &UartAddress);
	
	for(i = 0; i < UartLength; i++)
	{
		Status = WriteToFifo (INOUT pFifoOut, IN pUartData[i]);
		
		if(STATUS_SUCCESS == Status)
		{
			
		}
		else
		{
			return Status;
		}	
	}
	
	REGISTER_ADRESS_8(UartAddress.UCSRB) |= (1<<UDRIE3);
	
	return Status;			
}

void USART_TransmitByte(IN unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) )
	{
		
	}
	;
	/* Put data into buffer, sends the data */
	
	UDR1 = data;
}

unsigned char USART_Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) )
	;
	/* Get and return received data from buffer */
	return UDR1;
}