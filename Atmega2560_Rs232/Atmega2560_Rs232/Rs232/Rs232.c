/*
 * Rs232.c
 *
 * Created: 2015-09-01 19:13:12
 *  Author: Lukasz
 */
#include "Rs232.h"

UART_DEFINE()

//Funckja zwraca odpowiedni¹ kolejne FIFO dla danego UARTa oraz definiuje odpowidnie rejestry
void PickUart(IN UART_ENUM Uart, INOUT FIFO **pFifoIn, INOUT FIFO **pFifoOut, INOUT UART_ADDRESS *UartAddress)
{
   // Kolejka Fifo odbiorcza
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

   //Kolejka Fifo nadawcza
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

   //Definiowanie rejestrów
   if(UartAddress != NULL)
   {
      UART_REGISTER_DEFINE(UartAddress, Uart)
   }
}

//Inicjalizacja odpowiedniego UARTa
void USART_Init( IN uint32_t baud, IN UART_ENUM uart )
{
   unsigned int ubbr;
   FIFO *pFifoIn, *pFifoOut;
   UART_ADDRESS UartAddress;

   PickUart(IN uart, INOUT &pFifoIn, INOUT &pFifoOut, INOUT &UartAddress);

   //Inicjalizajca Fifo
   FifoInit(OUT pFifoOut);
   FifoInit(OUT pFifoIn);

   //Ustawienie szybkoœci transmisji
   ubbr = (F_CPU / (16 * baud)) - 1;
   REGISTER_ADRESS_8(UartAddress.UBBRH) = (unsigned char)(ubbr>>8);
   REGISTER_ADRESS_8(UartAddress.UBBRL) = (unsigned char)ubbr;

   //W³¹czenie przerwañ dla nadawanych i odbieranych danych
   REGISTER_ADRESS_8(UartAddress.UCSRB) = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
   REGISTER_ADRESS_8(UartAddress.UCSRB) &= ~(1<<UDRIE0);

   //Ustawienie formatu ramki
   REGISTER_ADRESS_8(UartAddress.UCSRC) = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) |
   (0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) | (0 << UMSEL00);
}

//Wpisanie wartoœci do kolejki Fifo
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

//Funckja realizuj¹ca odpowiednie zachowanie, jeœli bufor nadawania jest pusty
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
      //Wrzucenie kolejnych danych do bufora nadawczego
      REGISTER_ADRESS_8(UartAddress.UDR) = data;
   }
   else
   {
      REGISTER_ADRESS_8(UartAddress.UCSRB) &= ~(1<<UDRIE3); //wy³¹czemoe przerwania pustego bufora nadawania
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

//Funckja odczytyje dane z kolejki Fifo
STATUS USART_ReadByteFromFifo(IN UART_ENUM Uart, OUT unsigned char *pData)
{
   FIFO  *pFifoIn;

   PickUart(IN Uart, INOUT &pFifoIn, NULL, NULL);

   return ReadFromFifo (INOUT pFifoIn, IN pData);
}

//Funckja wrzuca pojedyñczy bajt do kolejki nadawczej
STATUS USART_TransmitByteByFifo( IN UART_ENUM Uart, IN unsigned char data)
{
   UART_ADDRESS UartAddress;
   FIFO  *pFifoOut;

   PickUart(IN Uart, NULL, INOUT &pFifoOut, INOUT &UartAddress);

   if(STATUS_SUCCESS == WriteToFifo (INOUT pFifoOut, IN data))
   {
      //w³¹czenie przerwania pustego bufora nadawania
      REGISTER_ADRESS_8(UartAddress.UCSRB) |= (1<<UDRIE3);

      return STATUS_SUCCESS;
   }
   else
   {
      return STATUS_FAILURE;
   }
}

//Funckja wrzuca dane do kolejki Fifo
STATUS USART_Transmit(IN UART_ENUM Uart, IN UART_LENGTH UartLength, IN unsigned char *pUartData)
{
   int i;
   UART_ADDRESS UartAddress;
   FIFO  *pFifoOut;
   STATUS Status;

   PickUart(IN Uart, NULL, INOUT &pFifoOut, INOUT &UartAddress);

   for(i = 0; i < UartLength; i++)
   {
      //Zapisywanie poszczególnych bajtów do kolejki Fifo
      Status = WriteToFifo (INOUT pFifoOut, IN pUartData[i]);

      if(STATUS_SUCCESS == Status)
      {

      }
      else
      {
         return Status;
      }
   }

   //w³¹czenie przerwania pustego bufora nadawania
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