/*
 * Fifo.c
 *
 * Created: 2015-09-01 19:39:05
 *  Author: Lukasz
 */ 

#include "Fifo.h"

void FifoInit(OUT FIFO *pFifo)
{
	pFifo->ReadPointer = 255;
	pFifo->WritePointer = 0;
		
	for(uint16_t i = 0; i < FIFO_SIZE; i++)
	{
		pFifo->Data[i] = 0;
	}
}

STATUS ReadFromFifo( INOUT FIFO *pFifo, OUT unsigned char *pData)
{
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

STATUS WriteToFifo( INOUT FIFO *pFifo, IN unsigned char pData)
{
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