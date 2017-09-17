#include "ADC.h"

#define ADC_VREF 4.76
#define ADC_MULTIPLE_1 1
#define ADC_MULTIPLE_2 2
#define ADC_MULTIPLE ADC_MULTIPLE_1

// Funkcja inicjuje konwerter analogowo cyfrowy ADC
void ADC_Init()
{
   // Wyb�r odpowiedniego napi�cia referencjnego w zale�no�ci od tego czy
   // zosta� u�yty dzielnik napi�cia
   if(ADC_MULTIPLE == ADC_MULTIPLE_2)
   {
      // AREF = 2,56V
      ADMUX = (1<<REFS0)|(1<<REFS1);
   }
   else if(ADC_MULTIPLE == ADC_MULTIPLE_1)
   {
      // AREF = AVcc czyli 5V
      ADMUX = (1<<REFS0);
   }

   // Uruchomienie ADC i ustawienie prescalera na warto�� 128
   // otrzymujemy waro�� cz�stotliwo�ci 16000000/128 = 125000
   ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

//Funckja odczytuje warto�� napiecia dla danego kana�y
STATUS ADC_Read(IN ADC_CHANNEL Channel, OUT ADC_RESPONSE *pADC)
{
   if(Channel >= 8)
   {
      Channel   &= 7;

      ADCSRB |= (1<<MUX5);
      ADMUX = (ADMUX & 0xF8)|Channel; // Wyzerowanie najni�szych 3 bit�w
   }
   else
   {
      ADCSRB &= ~(1<<MUX5);
      ADMUX = (ADMUX & 0xF8)|Channel; // Wyzerowanie najni�szych 3 bit�ws
   }

   // rozpocz�cie pojedy�czej transakcji
   // wpisanie 1 do ADSC
   ADCSRA |= (1<<ADSC);

   // je�li konwersja si� zako�czy
   // ADSC bedzie mia�o spowrotem waro�� 0
   while(ADCSRA & (1<<ADSC));

   *pADC = ADC;
   return (STATUS_SUCCESS);
}

// Funkcja zamienia warto�� odczytan� z ADC na konkretne napi�cie
STATUS ADC_ValueToVoltage(ADC_RESPONSE Adc, float *pVoltage)
{
   STATUS Status = STATUS_SUCCESS;
   float Temp = Adc;

   *pVoltage = (Temp / 1024) * ADC_VREF * ADC_MULTIPLE;

   return Status;
}