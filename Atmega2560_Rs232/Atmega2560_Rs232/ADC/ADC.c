/*
 * ADC.c
 *
 * Created: 2015-09-30 21:10:18
 *  Author: Lukasz
 */
#include "ADC.h"

#define ADC_VREF 4.53
#define ADC_MULTIPLE_1 1
#define ADC_MULTIPLE_2 2
#define ADC_MULTIPLE ADC_MULTIPLE_1

void ADC_Init()
{
	// AREF = AVcc
   if(ADC_MULTIPLE == ADC_MULTIPLE_2)
   {
	   ADMUX = (1<<REFS0)|(1<<REFS1);
   }
   else if(ADC_MULTIPLE == ADC_MULTIPLE_1)
   {
      ADMUX = (1<<REFS0);
   }            
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

STATUS ADC_Read(IN ADC_CHANNEL Channel, OUT ADC_RESPONSE *pADC)
{
	if(Channel >= 8)
	{
		Channel	 &= 7;
		
		ADCSRB |= (1<<MUX5);
		ADMUX = (ADMUX & 0xF8)|Channel; // clears the bottom 3 bits before ORing
	}
	else
	{
		ADCSRB &= ~(1<<MUX5);
		ADMUX = (ADMUX & 0xF8)|Channel; // clears the bottom 3 bits before ORing		
	}
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	*pADC = ADC;
	return (STATUS_SUCCESS);
}

STATUS ADC_ValueToVoltage(ADC_RESPONSE Adc, float *pVoltage)
{
   STATUS Status = STATUS_SUCCESS;
   float Temp = Adc;
   
   *pVoltage = (Temp / 1024) * ADC_VREF * ADC_MULTIPLE;
   
   return Status;
}