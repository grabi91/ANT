#ifndef ANT_MESSAGE_QUEUE_H_
#define ANT_MESSAGE_QUEUE_H_
#include "ANT_Framer.h"

STATUS ANT_Mesg_Q_Init();
STATUS ANT_Mesg_Q_ProcessPayload(ANT_MESSAGE_ITEM* pAntMessage);
#endif