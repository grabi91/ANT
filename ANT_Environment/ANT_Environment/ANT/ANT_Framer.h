#ifndef ANT_FRAMER_H_
#define ANT_FRAMER_H_

#include "../typedef.h"
#include "ANT_Defs.h"

typedef enum _ANT_FRAMER_MESSAGE_STATUS
{
	ANT_FMS_NOT_READY,
	ANT_FMS_READY,
	ANT_FMS_READ_OUT,
	ANT_FMS_LIMIT,
} ANT_FRAMER_MESSAGE_STATUS;

typedef struct _ANT_MESSAGE
{
	unsigned char MessageID;
	unsigned char Data[MESG_MAX_SIZE_VALUE];
} ANT_MESSAGE;

typedef struct _ANT_MESSAGE_ITEM
{
	unsigned char Size;
	ANT_MESSAGE AntMessage;
} ANT_MESSAGE_ITEM;

typedef struct _ANT_MESSAGE_FRAMER
{
	ANT_MESSAGE_ITEM AntItem;
	unsigned char ReadPointer;
	unsigned char ChechSumPartValue;
} ANT_MESSAGE_FRAMER;

typedef struct _ANT_MESSAGE_CONTEXT
{
	ANT_MESSAGE_FRAMER AntFramer;
	ANT_FRAMER_MESSAGE_STATUS MessageReady;
} ANT_MESSAGE_CONTEXT;

STATUS ANT_Framer_GetMessage(ANT_MESSAGE_ITEM *pAntMessage);
STATUS ANT_Framer_SendMessage(ANT_MESSAGE_ITEM* pAntMessage);

//////////////////////////////////////////////
// Message Definitions
//////////////////////////////////////////////
STATUS ANT_Framer_Mesg_ResetSystem();
STATUS ANT_Framer_Mesg_SetNetworkKey(unsigned char NetworkNumber, unsigned char *pKey, unsigned char KeySize);
STATUS ANT_Framer_Mesg_AssignChannel(unsigned char AntChannel, unsigned char ChannelType, unsigned char NetworkNumber);
STATUS ANT_Framer_Mesg_SetChannelID(unsigned char AntChannel, unsigned short DeviceNumber, unsigned char DeviceType, unsigned char TransmitType);
STATUS ANT_Framer_Mesg_SetChannelRFFrequency(unsigned char AntChannel, unsigned char RFFrequency);
STATUS ANT_Framer_Mesg_OpenChannel(unsigned char AntChannel);
STATUS ANT_Framer_Mesg_RxExtMesgsEnable(unsigned char Enable);
STATUS ANT_Framer_Mesg_SendBroadcastData(unsigned char AntChannel, unsigned char *pData);

#endif