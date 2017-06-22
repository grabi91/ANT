#include "ANT_Message_Queue.h"
#include "ANT_Framer.h"
#include "../Debug_MSG/Debug_Msg.h"

#define ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(Name) STATUS Name(ANT_MESSAGE_ITEM* pAntMessage)

#define ANT_MESG_Q_PP_TABLE_SIZE 0xFF

typedef ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE((*ANT_PayloadFunction));

ANT_PayloadFunction ANT_Mesg_Q_ProcessPayload_Table[ANT_MESG_Q_PP_TABLE_SIZE];


ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResetSystem);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_StartUp);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResponseEvent);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_NetworkKey);

STATUS ANT_Mesg_Q_Init()
{
   STATUS Status = STATUS_SUCCESS;

   ANT_Mesg_Q_ProcessPayload_Table[MESG_SYSTEM_RESET_ID] = ANT_Mesg_Q_PP_ResetSystem;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_STARTUP_MESG_ID] = ANT_Mesg_Q_PP_StartUp;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_RESPONSE_EVENT_ID] = ANT_Mesg_Q_PP_ResponseEvent;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_NETWORK_KEY_ID] = ANT_Mesg_Q_PP_NetworkKey;

   return Status;
}

STATUS ANT_Mesg_Q_ProcessPayload(ANT_MESSAGE_ITEM* pAntMessage)
{
   STATUS Status = STATUS_FAILURE;

   if (ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.MessageID] == 0)
   {
      Status = STATUS_MESSEGE_FUNCTION_NOT_DEFINE;
   }
   else
   {
      Status = (*ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.MessageID])(pAntMessage);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_StartUp)
{
   STATUS Status = STATUS_SUCCESS;

   DMsgMessageNewLine(25, "RESET Complete, reason: ");

   unsigned char Reason = pAntMessage->AntMessage.Data[0];

   if (Reason == RESET_POR)
      DMsgMessage(10, "RESET_POR");
   if (Reason & RESET_SUSPEND)
      DMsgMessage(15, "RESET_SUSPEND ");
   if (Reason & RESET_SYNC)
      DMsgMessage(12, "RESET_SYNC ");
   if (Reason & RESET_CMD)
      DMsgMessage(11, "RESET_CMD ");
   if (Reason & RESET_WDT)
      DMsgMessage(11, "RESET_WDT ");
   if (Reason & RESET_RST)
      DMsgMessage(11, "RESET_RST ");
   DMsgMessageNewLine(0, "");

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResetSystem)
{
   STATUS Status = STATUS_SUCCESS;

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResponseEvent)
{
   STATUS Status = STATUS_SUCCESS;

   if (ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.Data[1]] == 0)
   {
      Status = STATUS_MESSEGE_FUNCTION_NOT_DEFINE;
   }
   else
   {
      Status = (*ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.Data[1]])(pAntMessage);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_NetworkKey)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(37, "Error configuring network key: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(17, "Network key set.");
      DMsgMessageNewLine(21, "Assigning channel...");
   }

   return Status;
}

