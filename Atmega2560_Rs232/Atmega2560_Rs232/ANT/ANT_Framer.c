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
		UsartStatus = USART_ReadByteFromFifo(UART0, OUT &dataByte);
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
	STATUS Status = STATUS_SUCCESS;
	unsigned char data[] = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };

	USART_Transmit(UART0, sizeof(data), data);

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