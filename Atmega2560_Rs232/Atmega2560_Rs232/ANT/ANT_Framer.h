#ifndef ANT_FRAMER_H_
#define ANT_FRAMER_H_

#include "ANT_Defs.h"

typedef struct ANT_MESSAGE
{
	unsigned char ucMessageID;
	unsigned char aucData[MESG_MAX_SIZE_VALUE];
} ANT_MESSAGE;

typedef struct
{
	unsigned char ucSize;
	ANT_MESSAGE stANTMessage;
} ANT_MESSAGE_ITEM;

STATUS ANT_Framer_GetMessage(ANT_MESSAGE_ITEM* pAntMessage);


#endif