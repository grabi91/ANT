#include "../ANT/ANT_Message_Queue.h"
#include "../Debug_MSG/Debug_Msg.h"
#include "TemperatureSensor.h"

#define RESERVED 0xFF

#define NOT_REQUESTED 0
#define REQUESTED 1

#define INVALID_24H_TEMPERATURE 0x800
#define INVALID_TEMPERATURE 0x8000

#define BACKGROUND_PAGE_RATE 129

#define REQUEST_DATA_PAGE 0x01
#define REQUEST_ANT_FS_SESSION 0x02

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(HandleTransmit);
ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(HandleReceive);

typedef enum _TEMPERATURE_SENSOR_PAGE
{
   PAGE_0 = 0,
   PAGE_1 = 1,
   PAGE_70 = 70,
   PAGE_80 = 80,
   PAGE_81 = 81,
   PAGE_LAST_ENTRY,
} T_S_PAGE;

typedef struct _TEMPERATURE_SENSOR_REQUESTED_PAGE
{
   T_S_PAGE Page;
   uint8_t Requested;               // = 1 if requested
   uint8_t RequestCount;            //
} T_S_REQUESTED_PAGE;

typedef struct _TEMPERATURE_SENSOR_BACKGROUND_DATA_PAGE
{
   T_S_PAGE Page;
   uint8_t CountSinceLast;
} T_S_BACKGROUND_DATA_PAGE;

enum _DEFAULT_TRANSMISSION_RATE
{
   T_R_05HZ = 0,                    //default transmission rate of 0.5 Hz
   T_R_4HZ = 1,                     //default transmission rate of 4 Hz
} DEFAULT_TRANSMISSION_RATE;

enum _UTC_TIME
{
   UTC_NOT_SUPPORTED = 0,           //System Time not supported
   UTC_SUPPORTED_NOT_SET = 1,       //UTC Time Supported, Not Set
   UTC_SUPPORTED_SET = 2,           //UTC Time Supported & Set
} UTC_TIME;

enum _LOCAL_TIME
{
   LOCAL_NOT_SUPPORTED = 0,         //Local Time not supported
   LOCAL_SUPPORTED_NOT_SET = 1,     //Local Time Supported, Not Set
   LOCAL_SUPPORTED_SET = 2,         //Local Time Supported & Set
} LOCAL_TIME;

typedef union _TEMPERATURE_SENSOR_TRANSMISSION_INFO
{
   struct
   {
      uint8_t DefaultTransmissionRate : 2;
      uint8_t UTC_Time : 2;
      uint8_t LocalTime : 2;
      uint8_t Reserved : 2;
   };
   uint8_t Data;
} T_S_TRANSMISSION_INFO;

enum _SUPPORTED_PAGES
{
   SUPPORTED_PAGES_0 = 0x01,
   SUPPORTED_PAGES_1 = 0x02,
   SUPPORTED_PAGES_0_1 = 0x03,
} SUPPORTED_PAGES;

typedef struct _TEMPERATURE_DATA
{
   int16_t Low24h;                  //Signed Integer representing the lowest temperature recorded over 
                                    //the last 24 hours (0x800 invalid)
                                    //Unit 0.1°C
                                    //Range -204.7 to 204.7
   int16_t High24h;                 //Signed Integer representing the highest temperature recorded over 
                                    //the last 24 hours (0x800 invalid)
                                    //Unit 0.1°C
                                    //Range -204.7 to 204.7
   int16_t Current;                 //Signed Integer representing the current Temperature (0x8000 invalid)
                                    //Unit 0.01°C
                                    //Range -327.67 to 327.67
} TEMPERATURE_DATA;

typedef struct _MANUFACTURER_IDENTIFICATION
{
   uint8_t HWRevision;
   uint16_t ManufacturerId;
   uint16_t ModelNumber;
} MANUFACTURER_IDENTIFICATION;

typedef struct _PRODUCT_INFORMATION
{
   uint8_t SW_Revision;
   uint32_t SerialNumber;
} PRODUCT_INFORMATION;

typedef struct _TEMPERATURE_SENSOR_DATA_CONTEXT
{
   T_S_PAGE MainDataPage;
   T_S_BACKGROUND_DATA_PAGE BackgroundDataPage;
   T_S_REQUESTED_PAGE RequestedPage;
   T_S_TRANSMISSION_INFO TransmissionInfo;
   uint64_t SupportedPage[4];               //Bit Field representing main data pages supported.
                                    //Bit position refers to page number.
   uint32_t SupportedMainDataPages;         //Bit Field representing main data pages supported.
                                    //Bit position refers to page number.
                                    //Bits 2:31 – Reserved. Set to 0.
                                    //Bit 1 – Page 1 Support
                                    //Bit 0 – Page 0 Support
   uint8_t EventCount;
   TEMPERATURE_DATA Temperature;
   MANUFACTURER_IDENTIFICATION ManufacturerInformation;
   PRODUCT_INFORMATION ProductInformation;
} TEMPERATURE_SENSOR_DATA_CONTEXT;

TEMPERATURE_SENSOR_DATA_CONTEXT TemperatureData;

STATUS TemperatureSensorInit()
{
   STATUS Status;

   Status = ANT_Mesg_Q_RegisterMesgEvent(EVENT_TX, HandleTransmit, ANT_M_Q_R_FORCE);
   Status = ANT_Mesg_Q_RegisterMesgEvent(MESG_BROADCAST_DATA_ID, HandleReceive, ANT_M_Q_R_FORCE);
   Status = ANT_Mesg_Q_RegisterMesgEvent(MESG_ACKNOWLEDGED_DATA_ID, HandleReceive, ANT_M_Q_R_FORCE);

   TemperatureData.MainDataPage = PAGE_0;

   TemperatureData.BackgroundDataPage.Page = PAGE_80;
   TemperatureData.BackgroundDataPage.CountSinceLast = 0;

   TemperatureData.RequestedPage.Requested = NOT_REQUESTED;
   TemperatureData.RequestedPage.Page = PAGE_0;
   TemperatureData.RequestedPage.RequestCount = 0;

   TemperatureData.TransmissionInfo.DefaultTransmissionRate = T_R_4HZ;
   TemperatureData.TransmissionInfo.LocalTime = LOCAL_NOT_SUPPORTED;
   TemperatureData.TransmissionInfo.UTC_Time = UTC_NOT_SUPPORTED;
   TemperatureData.TransmissionInfo.Reserved = 0;

   TemperatureData.SupportedMainDataPages = SUPPORTED_PAGES_0_1;

   TemperatureData.EventCount = 0;

   TemperatureData.Temperature.Current = INVALID_TEMPERATURE;
   TemperatureData.Temperature.Low24h = INVALID_24H_TEMPERATURE;
   TemperatureData.Temperature.High24h = INVALID_24H_TEMPERATURE;

   TemperatureData.ManufacturerInformation.HWRevision = 0x01;
   TemperatureData.ManufacturerInformation.ManufacturerId = 0x0002;
   TemperatureData.ManufacturerInformation.ModelNumber = 0x0003;

   TemperatureData.ProductInformation.SerialNumber = 0x00000004;
   TemperatureData.ProductInformation.SW_Revision = 0x05;

   return Status;
}

STATUS EncodeData(T_S_PAGE Page, uint8_t *pData, uint8_t DataLength)
{
   STATUS Status = STATUS_SUCCESS;

   if (DataLength < MESG_ANT_MAX_PAYLOAD_SIZE)
   {
      Status = STATUS_DATA_BUFFER_TO_SHORT;
   }

   if (Status == STATUS_SUCCESS)
   {
      switch (Page)
      {
      case PAGE_0:
         pData[0] = Page;
         pData[1] = RESERVED;
         pData[2] = RESERVED;
         pData[3] = TemperatureData.TransmissionInfo.Data;
         pData[4] = TemperatureData.SupportedMainDataPages;
         pData[5] = 0x00;
         pData[6] = 0x00;
         pData[7] = 0x00;
         break;
      case PAGE_1:
         pData[0] = Page;
         pData[1] = RESERVED;
         pData[2] = TemperatureData.EventCount;
         pData[3] = TemperatureData.Temperature.Low24h & BYTE_MASK;
         pData[4] = (TemperatureData.Temperature.Low24h >> 4) & UPPER_NIBBLE_MASK;
         pData[4] = pData[4] | (TemperatureData.Temperature.High24h & LOWER_NIBBLE_MASK);
         pData[5] = (TemperatureData.Temperature.High24h >> 4) & BYTE_MASK;
         pData[6] = TemperatureData.Temperature.Current & BYTE_MASK;
         pData[7] = (TemperatureData.Temperature.Current >> 8) & BYTE_MASK;
         break;
      case PAGE_80:
         pData[0] = Page;
         pData[1] = RESERVED;
         pData[2] = RESERVED;
         pData[3] = TemperatureData.ManufacturerInformation.HWRevision;
         pData[4] = TemperatureData.ManufacturerInformation.ManufacturerId & BYTE_MASK;
         pData[5] = (TemperatureData.ManufacturerInformation.ManufacturerId >> 8) & BYTE_MASK;
         pData[6] = TemperatureData.ManufacturerInformation.ModelNumber & BYTE_MASK;
         pData[7] = (TemperatureData.ManufacturerInformation.ModelNumber >> 8) & BYTE_MASK;
         break;
      case PAGE_81:
         pData[0] = Page;
         pData[1] = RESERVED;
         pData[2] = RESERVED;
         pData[3] = TemperatureData.ProductInformation.SW_Revision;
         pData[4] = TemperatureData.ProductInformation.SerialNumber & BYTE_MASK;
         pData[5] = (TemperatureData.ProductInformation.SerialNumber >> 8) & BYTE_MASK;
         pData[6] = (TemperatureData.ProductInformation.SerialNumber >> 16) & BYTE_MASK;
         pData[7] = (TemperatureData.ProductInformation.SerialNumber >> 24) & BYTE_MASK;
         break;
      default:
         Status = STATUS_PAGE_UNSUPPORTED;
         break;
      }
   }

   return Status;
}

STATUS DecodeData(uint8_t *pData, uint8_t DataLength)
{
   STATUS Status = STATUS_SUCCESS;

   if (DataLength < MESG_ANT_MAX_PAYLOAD_SIZE)
   {
      Status = STATUS_DATA_BUFFER_TO_SHORT;
   }

   if (Status == STATUS_SUCCESS)
   {
      switch (pData[0])
      {
      case PAGE_70:
         if (((pData[5] & 0x7F) != 0) && (pData[7] == REQUEST_DATA_PAGE))
         {
            for (T_S_PAGE i = PAGE_0; i < PAGE_LAST_ENTRY; i++)
            {
               if (pData[6] == i)
               {
                  TemperatureData.RequestedPage.Page = pData[6];
                  TemperatureData.RequestedPage.RequestCount = (pData[5] & 0x7F);
                  TemperatureData.RequestedPage.Requested = REQUESTED;

                  break;
               }
            }
         }
         break;
      default:
         Status = STATUS_PAGE_UNSUPPORTED;
         break;
      }
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(HandleTransmit)
{
   STATUS Status = STATUS_SUCCESS;
   uint8_t DataToSend[MESG_ANT_MAX_PAYLOAD_SIZE];
   uint8_t DataToSendLength = MESG_ANT_MAX_PAYLOAD_SIZE;
   T_S_PAGE PageToSend = PAGE_0;

   DMsgMessageNewLine(9, (unsigned char*)"EventTx ");

   //Pick page to send
   if (TemperatureData.RequestedPage.Requested == REQUESTED)
   {
      PageToSend = TemperatureData.RequestedPage.Page;
      TemperatureData.RequestedPage.RequestCount--;

      if (TemperatureData.RequestedPage.RequestCount == 0)
      {
         TemperatureData.RequestedPage.Requested = NOT_REQUESTED;
      }
   }
   else if (TemperatureData.BackgroundDataPage.CountSinceLast == BACKGROUND_PAGE_RATE)
   {
      switch (TemperatureData.BackgroundDataPage.Page)
      {
      case PAGE_80:
         TemperatureData.BackgroundDataPage.CountSinceLast = 0xFF;
         TemperatureData.BackgroundDataPage.Page = PAGE_81;
         PageToSend = PAGE_80;
         break;
      case PAGE_81:
         TemperatureData.BackgroundDataPage.CountSinceLast = 0xFF;
         TemperatureData.BackgroundDataPage.Page = PAGE_80;
         PageToSend = PAGE_81;
         break;
      }
   }
   else
   {
      switch (TemperatureData.MainDataPage)
      {
      case PAGE_0:
         TemperatureData.MainDataPage = PAGE_1;
         PageToSend = PAGE_0;
         break;
      case PAGE_1:
         TemperatureData.MainDataPage = PAGE_0;
         PageToSend = PAGE_1;
         break;
      }
   }

   if (Status == STATUS_SUCCESS)
   {
      //Prepare Data
      Status = EncodeData(PageToSend, &DataToSend, DataToSendLength);
   }

   if (Status == STATUS_SUCCESS)
   {
      //Send Data
      Status = ANT_Framer_Mesg_SendBroadcastData(USER_ANTCHANNEL, &DataToSend);

      for (int i = 0; i < DataToSendLength; i++)
      {
         unsigned char temp[6];
         sprintf(temp, "0x%02x ", DataToSend[i]);
         DMsgMessage(sizeof(temp), temp);
      }
   }

   if (Status == STATUS_SUCCESS)
   {
      TemperatureData.EventCount++;
      TemperatureData.BackgroundDataPage.CountSinceLast++;
   }

   return Status;
}

ANT_MESG_Q_PP_TABLE_FUNC_TEMPLATE(HandleReceive)
{
   STATUS Status = STATUS_SUCCESS;

   DMsgMessageNewLine(9, (unsigned char*)"EventRx ");

   for (int i = 0; i < pAntMessage->Size; i++)
   {
      unsigned char temp[6];
      sprintf(temp, "0x%02x ", pAntMessage->AntMessage.Data[i]);
      DMsgMessage(sizeof(temp), temp);
   }

   Status = DecodeData(&(pAntMessage->AntMessage.Data[1]), pAntMessage->Size - 1);

   return Status;
}