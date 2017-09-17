#include "ADC.h"

#define ADC_VREF 4.76
#define ADC_MULTIPLE_1 1
#define ADC_MULTIPLE_2 2
#define ADC_MULTIPLE ADC_MULTIPLE_1

// Funkcja inicjuje konwerter analogowo cyfrowy ADC
void ADC_Init()
{
   // Wybór odpowiedniego napiêcia referencjnego w zale¿noœci od tego czy
   // zosta³ u¿yty dzielnik napiêcia
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

   // Uruchomienie ADC i ustawienie prescalera na wartoœæ 128
   // otrzymujemy waroœæ czêstotliwoœci 16000000/128 = 125000
   ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

//Funckja odczytuje wartoœæ napiecia dla danego kana³y
STATUS ADC_Read(IN ADC_CHANNEL Channel, OUT ADC_RESPONSE *pADC)
{
   if(Channel >= 8)
   {
      Channel   &= 7;

      ADCSRB |= (1<<MUX5);
      ADMUX = (ADMUX & 0xF8)|Channel; // Wyzerowanie najni¿szych 3 bitów
   }
   else
   {
      ADCSRB &= ~(1<<MUX5);
      ADMUX = (ADMUX & 0xF8)|Channel; // Wyzerowanie najni¿szych 3 bitóws
   }

   // rozpoczêcie pojedyñczej transakcji
   // wpisanie 1 do ADSC
   ADCSRA |= (1<<ADSC);

   // jeœli konwersja siê zakoñczy
   // ADSC bedzie mia³o spowrotem waroœæ 0
   while(ADCSRA & (1<<ADSC));

   *pADC = ADC;
   return (STATUS_SUCCESS);
}

// Funkcja zamienia wartoœæ odczytan¹ z ADC na konkretne napiêcie
STATUS ADC_ValueToVoltage(ADC_RESPONSE Adc, float *pVoltage)
{
   STATUS Status = STATUS_SUCCESS;
   float Temp = Adc;

   *pVoltage = (Temp / 1024) * ADC_VREF * ADC_MULTIPLE;

   return Status;
}