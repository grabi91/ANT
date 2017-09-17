#include "..\typedef.h"
#include "..\FIFO\Fifo.h"

#ifndef RS232_H_
#define RS232_H_

typedef enum _UART_ENUM{
   UART0 = 0xC0,
   UART1 = 0xC8,
   UART2 = 0xD0,
   UART3 = 0x130
} UART_ENUM;

typedef struct _UART_ADDRESS{
   uint16_t UCSRA;
   uint16_t UCSRB;
   uint16_t UCSRC;
   uint16_t Reserved3;
   uint16_t UBBRL;
   uint16_t UBBRH;
   uint16_t UDR;
   uint16_t Reserved7;
}UART_ADDRESS;

typedef uint8_t UART_LENGTH;

//definiowane odpowiednich rejestrów
#define UART_REGISTER_DEFINE(xUartAddress, xUart)  \
   xUartAddress->UCSRA = xUart;                    \
   xUartAddress->UCSRB = xUart+1;                  \
   xUartAddress->UCSRC = xUart+2;                  \
   xUartAddress->Reserved3 = xUart+3;              \
   xUartAddress->UBBRL = xUart+4;                  \
   xUartAddress->UBBRH = xUart+5;                  \
   xUartAddress->UDR = xUart+6;                    \
   xUartAddress->Reserved7 = xUart+7;

//definiowane kolejek fifo do uart
#define UART_DEFINE(x)        \
   x FIFO UART0FifoIn;        \
   x FIFO UART0FifoOut;       \
                              \
   x FIFO UART1FifoIn;        \
   x FIFO UART1FifoOut;       \
                              \
   x FIFO UART2FifoIn;        \
   x FIFO UART2FifoOut;       \
                              \
   x FIFO UART3FifoIn;        \
   x FIFO UART3FifoOut;

void USART_Init( IN uint32_t baud, IN UART_ENUM uart );
STATUS USART_TransmitByteByFifo(IN UART_ENUM Uart, IN unsigned char data);
STATUS USART_Transmit(IN UART_ENUM Uart, IN UART_LENGTH UartLength, IN unsigned char *pUartData);
STATUS USART_ReadByteFromFifo(IN UART_ENUM Uart, OUT unsigned char *pData);
void USART_TransmitByte(IN unsigned char data);

#endif /* RS232_H_ */