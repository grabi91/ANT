#pragma once
typedef enum _UART_ENUM {
	UART0 = 0xC0,
	UART1 = 0xC8,
	UART2 = 0xD0,
	UART3 = 0x130
} UART_ENUM;

typedef uint8_t UART_LENGTH;

void USART_Init(uint32_t baud, UART_ENUM uart);
void USART_Close(UART_ENUM uart);
STATUS USART_TransmitByteByFifo(IN UART_ENUM Uart, IN unsigned char data);
//STATUS USART_Transmit(IN UART_ENUM Uart, IN UART_LENGTH UartLength, IN unsigned char *pUartData);
STATUS USART_ReadByteFromFifo(IN UART_ENUM Uart, OUT unsigned char *pData);
