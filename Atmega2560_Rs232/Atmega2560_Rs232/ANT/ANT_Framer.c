#include "ANT_Framer.h"
#ifdef WIN32
#include "../../../Implementacja_1/Implementacja_1/UART.h"
#else
#include "../Rs232/Rs232.h"
#endif

ANT_MESSAGE_CONTEXT ANT_Framer_Context;

unsigned char ANT_Framer_CheckSum_Calc8(unsigned char *pData, unsigned char DataSize, unsigned char ChechSumStartValue)
{
	unsigned char CheckSum = ChechSumStartValue;
	int i;

	// Calculate the CheckSum value (XOR of all bytes in the buffer).
	for (i = 0; i < DataSize; i++)
		CheckSum ^= pData[i];

	return CheckSum;
}

STATUS ANT_Framer_SendClearMessage()
{
	STATUS Status = STATUS_SUCCESS;

	memset(&ANT_Framer_Context.AntFramer, 0, sizeof(ANT_Framer_Context.AntFramer));
	ANT_Framer_Context.MessageReady = ANT_FMS_NOT_READY;

	return Status;
}

STATUS ANT_Framer_CheckNewByte()
{
	STATUS Status = STATUS_SUCCESS;
	STATUS UsartStatus = STATUS_SUCCESS;
	unsigned char dataByte = 0;
	unsigned char CheckSum = 0;

	if (ANT_Framer_Context.MessageReady == ANT_FMS_NOT_READY)
	{
		UsartStatus = USART_ReadByteFromFifo(UART2, OUT &dataByte);
		if (UsartStatus == STATUS_SUCCESS)
		{
			if (ANT_Framer_Context.AntFramer.ReadPointer == MESG_SYNC_OFFSET)
			{
				if (dataByte == MESG_TX_SYNC)
				{
					ANT_Framer_Context.AntFramer.ReadPointer++;
					ANT_Framer_Context.AntFramer.ChechSumPartValue = ANT_Framer_CheckSum_Calc8(&dataByte, sizeof(dataByte), ANT_Framer_Context.AntFramer.ChechSumPartValue);
				}
			}
			else if (ANT_Framer_Context.AntFramer.ReadPointer == MESG_SIZE_OFFSET)
			{
				ANT_Framer_Context.AntFramer.ReadPointer++;
				ANT_Framer_Context.AntFramer.AntItem.Size = dataByte;
				ANT_Framer_Context.AntFramer.ChechSumPartValue = ANT_Framer_CheckSum_Calc8(&dataByte, sizeof(dataByte), ANT_Framer_Context.AntFramer.ChechSumPartValue);
			}
			else if (ANT_Framer_Context.AntFramer.ReadPointer == MESG_ID_OFFSET)
			{
				ANT_Framer_Context.AntFramer.ReadPointer++;
				ANT_Framer_Context.AntFramer.AntItem.AntMessage.MessageID = dataByte;
				ANT_Framer_Context.AntFramer.ChechSumPartValue = ANT_Framer_CheckSum_Calc8(&dataByte, sizeof(dataByte), ANT_Framer_Context.AntFramer.ChechSumPartValue);
			}
			else if (ANT_Framer_Context.AntFramer.ReadPointer == MESG_CHECKSUM_OFFSET(ANT_Framer_Context.AntFramer.AntItem.Size))
			{
				CheckSum = ANT_Framer_CheckSum_Calc8(ANT_Framer_Context.AntFramer.AntItem.AntMessage.Data, ANT_Framer_Context.AntFramer.AntItem.Size, ANT_Framer_Context.AntFramer.ChechSumPartValue);

				if (CheckSum == dataByte)
				{
					ANT_Framer_Context.MessageReady = ANT_FMS_READY;
				}
			}
			else
			{				
				ANT_Framer_Context.AntFramer.AntItem.AntMessage.Data[ANT_Framer_Context.AntFramer.ReadPointer - MESG_DATA_OFFSET] = dataByte;
				ANT_Framer_Context.AntFramer.ReadPointer++;
			}
		}
	}

	return Status;
}

STATUS ANT_Framer_SendMessage(ANT_MESSAGE_ITEM* pAntMessage)
{
	STATUS Status = STATUS_FAILURE;
	//unsigned char data[] = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };
	unsigned char DataToSend[MESG_MAX_SIZE_VALUE]; //TODO change buffer size
	unsigned char DataSize = 0;

	DataToSend[DataSize] = MESG_TX_SYNC;
	DataSize++;

	DataToSend[DataSize] = pAntMessage->Size;
	DataSize++;

	DataToSend[DataSize] = pAntMessage->AntMessage.MessageID;
	DataSize++;

	memcpy(&DataToSend[DataSize], pAntMessage->AntMessage.Data, pAntMessage->Size);
	DataSize += pAntMessage->Size;

	DataToSend[DataSize] = ANT_Framer_CheckSum_Calc8(DataToSend, DataSize, 0);
	DataSize++;

	DataToSend[DataSize] = 0;
	DataSize++;
	DataToSend[DataSize] = 0;
	DataSize++;

	Status = USART_Transmit(UART2, DataSize, DataToSend);

	return Status;
}

STATUS ANT_Framer_GetMessage(ANT_MESSAGE_ITEM* pAntMessage)
{
	STATUS Status = STATUS_FAILURE;

	ANT_Framer_CheckNewByte();
	
	if (ANT_Framer_Context.MessageReady == ANT_FMS_READY)
	{
		memcpy(pAntMessage, &ANT_Framer_Context.AntFramer.AntItem, sizeof(ANT_MESSAGE_ITEM));
		Status = ANT_Framer_SendClearMessage();
	}

	return Status;
}

//////////////////////////////////////////////
// Message Definitions
//////////////////////////////////////////////
STATUS ANT_Framer_Mesg_ResetSystem()
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_SYSTEM_RESET_ID;
   AntMessage.AntMessage.Data[0] = 0;
   AntMessage.Size = MESG_SYSTEM_RESET_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_SetNetworkKey(unsigned char NetworkNumber, unsigned char *pKey, unsigned char KeySize)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_NETWORK_KEY_ID;
   AntMessage.AntMessage.Data[0] = NetworkNumber;
   memcpy(&AntMessage.AntMessage.Data[1], pKey, KeySize);
   AntMessage.Size = MESG_NETWORK_KEY_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_AssignChannel(unsigned char AntChannel, unsigned char ChannelType, unsigned char NetworkNumber)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_ASSIGN_CHANNEL_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   AntMessage.AntMessage.Data[1] = ChannelType;
   AntMessage.AntMessage.Data[2] = NetworkNumber;
   AntMessage.Size = MESG_ASSIGN_CHANNEL_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_SetChannelID(unsigned char AntChannel, unsigned short DeviceNumber, unsigned char DeviceType, unsigned char TransmitType)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_CHANNEL_ID_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   AntMessage.AntMessage.Data[1] = (DeviceNumber & 0xFF);
   AntMessage.AntMessage.Data[2] = ((DeviceNumber >> 8) & 0xFF);
   AntMessage.AntMessage.Data[3] = DeviceType;
   AntMessage.AntMessage.Data[4] = TransmitType;
   AntMessage.Size = MESG_CHANNEL_ID_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_SetChannelPeriod(unsigned char AntChannel, unsigned short ChannelPeriod)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_CHANNEL_MESG_PERIOD_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   AntMessage.AntMessage.Data[1] = (ChannelPeriod & 0xFF);
   AntMessage.AntMessage.Data[2] = ((ChannelPeriod >> 8) & 0xFF);
   AntMessage.Size = MESG_CHANNEL_MESG_PERIOD_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_SetChannelRFFrequency(unsigned char AntChannel, unsigned char RFFrequency)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_CHANNEL_RADIO_FREQ_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   AntMessage.AntMessage.Data[1] = RFFrequency;
   AntMessage.Size = MESG_CHANNEL_RADIO_FREQ_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_OpenChannel(unsigned char AntChannel)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_OPEN_CHANNEL_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   AntMessage.Size = MESG_OPEN_CHANNEL_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_RxExtMesgsEnable(unsigned char Enable)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_RX_EXT_MESGS_ENABLE_ID;
   AntMessage.AntMessage.Data[0] = 0;
   AntMessage.AntMessage.Data[1] = Enable;
   AntMessage.Size = MESG_RX_EXT_MESGS_ENABLE_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}

STATUS ANT_Framer_Mesg_SendBroadcastData(unsigned char AntChannel, unsigned char *pData)
{
   STATUS Status = STATUS_FAILURE;
   ANT_MESSAGE_ITEM AntMessage;

   AntMessage.AntMessage.MessageID = MESG_BROADCAST_DATA_ID;
   AntMessage.AntMessage.Data[0] = AntChannel;
   memcpy(&AntMessage.AntMessage.Data[1], pData, ANT_STANDARD_DATA_PAYLOAD_SIZE);
   AntMessage.Size = ANT_STANDARD_DATA_PAYLOAD_SIZE + MESG_CHANNEL_NUM_SIZE;

   Status = ANT_Framer_SendMessage(&AntMessage);

   return Status;
}