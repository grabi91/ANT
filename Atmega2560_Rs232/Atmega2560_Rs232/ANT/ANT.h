/*
 * ANT.h
 *
 * Created: 2015-11-12 14:30:09
 *  Author: Lukasz
 */

#include "..\typedef.h" 
#include "..\Rs232\RS232.h"


#ifndef ANT_H_
#define ANT_H_

#define UART_ANT	UART2

#define ANT_CH_ID    0x00
#define ANT_CH_TYPE  0x10
#define ANT_NET_ID   0x00
#define ANT_DEV_ID1  0x00
#define ANT_DEV_ID2  0x00
#define ANT_DEV_TYPE 0x00
#define ANT_TX_TYPE  0x00
#define ANT_CH_FREQ  0x0039
#define ANT_CH_PER   0x1f86

void ANT_Reset();
void ANT_AssignChannel();

#endif /* ANT_H_ */