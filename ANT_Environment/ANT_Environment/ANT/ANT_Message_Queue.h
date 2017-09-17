#ifndef ANT_MESSAGE_QUEUE_H_
#define ANT_MESSAGE_QUEUE_H_
#include "ANT_Framer.h"

#define ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(Name) STATUS Name(ANT_MESSAGE_ITEM* pAntMessage)

typedef ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE((*ANT_PayloadFunction));

typedef enum _ANT_MESG_Q_REG_OPTION
{
   ANT_M_Q_R_STANDARD,
   ANT_M_Q_R_FORCE,
} ANT_MESG_Q_REG_OPTION;

STATUS ANT_Mesg_Q_Init();
STATUS ANT_Mesg_Q_RegisterMesgEvent(uint8_t MesgEventId, ANT_PayloadFunction FunctionPointer, ANT_MESG_Q_REG_OPTION ForceRegister);
STATUS ANT_Mesg_Q_ProcessPayload(ANT_MESSAGE_ITEM* pAntMessage);
#endif