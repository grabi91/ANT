/*
 * Fifo.c
 *
 * Created: 2015-09-01 19:39:05
 *  Author: Lukasz
 */

#include "Fifo.h"

//Inicjalizacja kolejki Fifo
void FifoInit(OUT FIFO *pFifo)
{
   //Odpowiednie ustawanie pointerów odczyty i zapisu
   pFifo->ReadPointer = 255;
   pFifo->WritePointer = 0;

   //Wyzerowanie kolejki
   for(uint16_t i = 0; i < FIFO_SIZE; i++)
   {
      pFifo->Data[i] = 0;
   }
}

//Odczyt bajtu z kolejki
STATUS ReadFromFifo( INOUT FIFO *pFifo, OUT unsigned char *pData)
{
   //Sprawdzenie czy są jakieś dane w kolejce
   if((uint8_t)(pFifo->ReadPointer+1) != pFifo->WritePointer)
   {
      *pData = pFifo->Data[(uint8_t)(pFifo->ReadPointer+1)];
      pFifo->ReadPointer++;

      return STATUS_SUCCESS;
   }
   else
   {
      return STATUS_FAILURE;
   }
}

//Zapis bajtów do kolejki
STATUS WriteToFifo( INOUT FIFO *pFifo, IN unsigned char pData)
{
   //Sprawdzenie czy jest wolne miejsce w kolejce
   if(pFifo->WritePointer != pFifo->ReadPointer)
   {
      pFifo->Data[pFifo->WritePointer] = pData;
      pFifo->WritePointer++;

      return STATUS_SUCCESS;
   }
   else
   {
      return STATUS_FAILURE;
   }
}