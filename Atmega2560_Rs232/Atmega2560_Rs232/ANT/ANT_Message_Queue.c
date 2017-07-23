#include "ANT_Message_Queue.h"
#include "../Debug_MSG/Debug_Msg.h"

#define ANT_MESG_Q_PP_TABLE_SIZE 0xFF

ANT_PayloadFunction ANT_Mesg_Q_ProcessPayload_Table[ANT_MESG_Q_PP_TABLE_SIZE];


ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResetSystem);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_StartUp);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_ResponseEvent);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_NetworkKey);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_AssignChannel);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelID);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelPeriod);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelRFFrequency);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_OpenChannel);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_RxExtMesgsEnable);

STATUS ANT_Mesg_Q_Init()
{
   STATUS Status = STATUS_SUCCESS;

   ANT_Mesg_Q_ProcessPayload_Table[MESG_SYSTEM_RESET_ID] = ANT_Mesg_Q_PP_ResetSystem;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_STARTUP_MESG_ID] = ANT_Mesg_Q_PP_StartUp;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_RESPONSE_EVENT_ID] = ANT_Mesg_Q_PP_ResponseEvent;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_NETWORK_KEY_ID] = ANT_Mesg_Q_PP_NetworkKey;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_ASSIGN_CHANNEL_ID] = ANT_Mesg_Q_PP_AssignChannel;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_CHANNEL_ID_ID] = ANT_Mesg_Q_PP_SetChannelID;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_CHANNEL_MESG_PERIOD_ID] = ANT_Mesg_Q_PP_SetChannelPeriod;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_CHANNEL_RADIO_FREQ_ID] = ANT_Mesg_Q_PP_SetChannelRFFrequency;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_OPEN_CHANNEL_ID] = ANT_Mesg_Q_PP_OpenChannel;
   ANT_Mesg_Q_ProcessPayload_Table[MESG_RX_EXT_MESGS_ENABLE_ID] = ANT_Mesg_Q_PP_RxExtMesgsEnable;

   return Status;
}

STATUS ANT_Mesg_Q_RegisterMesgEvent(uint8_t MesgEventId, ANT_PayloadFunction FunctionPointer, ANT_MESG_Q_REG_OPTION ForceRegister)
{
   STATUS Status = STATUS_SUCCESS;

   if (ForceRegister == ANT_M_Q_R_STANDARD)
   {
      if (ANT_Mesg_Q_ProcessPayload_Table[MesgEventId] != 0)
      {
         Status = STATUS_MESSEGE_FUNCTION_ALREADY_DEFINE;
      }
   }

   if (Status == STATUS_SUCCESS)
   {
      ANT_Mesg_Q_ProcessPayload_Table[MesgEventId] = FunctionPointer;
   }

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
   uint8_t EventByteNumber = 0;

   if (pAntMessage->AntMessage.Data[1] == 1)
   {
      //Channel Event
      EventByteNumber = 2;
   }
   else
   {
      //Channel Respone
      EventByteNumber = 1;
   }


   if (ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.Data[EventByteNumber]] == 0)
   {
      Status = STATUS_MESSEGE_FUNCTION_NOT_DEFINE;
   }
   else
   {
      Status = (*ANT_Mesg_Q_ProcessPayload_Table[pAntMessage->AntMessage.Data[EventByteNumber]])(pAntMessage);
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

      Status = ANT_Framer_Mesg_AssignChannel(USER_ANTCHANNEL, PARAMETER_TX_NOT_RX, USER_NETWORK_NUM);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_AssignChannel)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(31, "Error assigning channel: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(18, "Channel assigned.");
      DMsgMessageNewLine(22, "Setting Channel ID...");

      Status = ANT_Framer_Mesg_SetChannelID(USER_ANTCHANNEL, USER_DEVICENUM, USER_DEVICETYPE, USER_TRANSTYPE);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelID)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(32, "Error setting channel Id: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(16, "Channel Id set.");
      DMsgMessageNewLine(27, "Setting Channel Period...");

      Status = ANT_Framer_Mesg_SetChannelPeriod(USER_ANTCHANNEL, USER_CHANNEL_PERIOD);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelPeriod)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(32, "Error setting channel period: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(16, "Channel Period set.");
      DMsgMessageNewLine(27, "Setting Radio Frequency...");

      Status = ANT_Framer_Mesg_SetChannelRFFrequency(USER_ANTCHANNEL, USER_RADIOFREQ);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_SetChannelRFFrequency)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(34, "Error setting RF Frequency: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(18, "RF Frequency set.");
      DMsgMessageNewLine(19, "Opening channel...");

      Status = ANT_Framer_Mesg_OpenChannel(USER_ANTCHANNEL);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_OpenChannel)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(34, "Error opening channel: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(18, "Channel open.");
      DMsgMessageNewLine(19, "Enabling extended message...");

      //Status = ANT_Framer_Mesg_RxExtMesgsEnable(1);
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(ANT_Mesg_Q_PP_RxExtMesgsEnable)
{
   STATUS Status = STATUS_SUCCESS;

   if (pAntMessage->AntMessage.Data[2] == INVALID_MESSAGE)
   {
      DMsgMessageNewLine(52, "Extended messages not supported in this ANT product");
      Status = STATUS_FAILURE;
   }
   else if (pAntMessage->AntMessage.Data[2] != RESPONSE_NO_ERROR)
   {
      DMsgMessageNewLine(39, "Error Enabling extended message: Code ");
      Status = STATUS_FAILURE;
   }
   else
   {
      DMsgMessageNewLine(27, "Extended messages enabled.");
   }

   return Status;
}