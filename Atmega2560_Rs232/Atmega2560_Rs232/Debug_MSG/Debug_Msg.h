#include "..\typedef.h"
#ifdef WIN32
#else
#include "..\Rs232\RS232.h"
#endif

#ifndef DEBUG_MSG_H_
#define DEBUG_MSG_H_

typedef uint8_t DMsg_LENGTH;

#define DEBUG_MSG
#define UART_DMsg    UART3

STATUS DMsgMessage(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage);
STATUS DMsgMessageNewPage(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage);
STATUS DMsgMessageNewLine(IN DMsg_LENGTH MessageLength, IN unsigned char *pMessage);

#endif /* DEBUG_MSG_H_ */