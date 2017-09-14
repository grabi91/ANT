/*
 * Fifo.h
 *
 * Created: 2015-09-01 19:40:47
 *  Author: Lukasz
 */
#include "..\typedef.h"


#ifndef FIFO_H_
#define FIFO_H_

#define FIFO_SIZE 256

typedef struct _FIFO {
   uint8_t ReadPointer;
   uint8_t WritePointer;
   uint8_t Data[FIFO_SIZE];
}FIFO;

void FifoInit(OUT FIFO *pFifo);
STATUS ReadFromFifo( INOUT FIFO *pFifo, OUT unsigned char *pData);
STATUS WriteToFifo( INOUT FIFO *pFifo, IN unsigned char pData);


#endif /* FIFO_H_ */