/*
 * RS232_Podstawy.c
 *
 * Created: 2015-08-23 18:45:00
 *  Author: Lukasz
 */ 

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)| (1<<UCSZ1) | (1<<UCSZ0); 
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}


/*int main(void)
{
	char data;
	
	USART_Init( 51 );
	while(1)
	{
		data=USART_Receive();
		
		USART_Transmit('[');
		USART_Transmit(data);
		USART_Transmit(']');
	}
}*/