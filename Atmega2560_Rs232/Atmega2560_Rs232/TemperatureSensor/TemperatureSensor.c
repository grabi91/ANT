#include "../ANT/ANT_Message_Queue.h"
#include "../Debug_MSG/Debug_Msg.h"
#include "../ADC/ADC.h"
#include "TemperatureSensor.h"

#define RESERVED 0xFF

#define NOT_REQUESTED 0
#define REQUESTED 1

#define INVALID_24H_TEMPERATURE 0x800
#define INVALID_TEMPERATURE 0x8000

#define BACKGROUND_PAGE_RATE 129

#define REQUEST_DATA_PAGE 0x01
#define REQUEST_ANT_FS_SESSION 0x02

#define ADC_BUFFER_SIZE 15

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

typedef enum _TEMPERATURE_DATA_SWITCH
{
   SWITCH_0,
   SWITCH_1,
   SWTICH_LIMIT,
} TEMPERATURE_DATA_SWITCH;

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

typedef struct _ADC_DATA
{
   uint8_t Size;
   uint8_t WritePointer;
   ADC_RESPONSE Data[ADC_BUFFER_SIZE];
} ADC_DATA;

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
   TEMPERATURE_DATA_SWITCH TemperatureSwitch;
   TEMPERATURE_DATA Temperature[SWTICH_LIMIT];
   MANUFACTURER_IDENTIFICATION ManufacturerInformation;
   PRODUCT_INFORMATION ProductInformation;
   ADC_DATA Adc;
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

   TemperatureData.TemperatureSwitch = SWITCH_0;
   for(TEMPERATURE_DATA_SWITCH i = SWITCH_0; i < SWTICH_LIMIT; i++)
   {
      TemperatureData.Temperature[i].Current = INVALID_TEMPERATURE;
      TemperatureData.Temperature[i].Low24h = INVALID_24H_TEMPERATURE;
      TemperatureData.Temperature[i].High24h = INVALID_24H_TEMPERATURE;
   }

   TemperatureData.ManufacturerInformation.HWRevision = 0x01;
   TemperatureData.ManufacturerInformation.ManufacturerId = 0x0002;
   TemperatureData.ManufacturerInformation.ModelNumber = 0x0003;

   TemperatureData.ProductInformation.SerialNumber = 0x00000004;
   TemperatureData.ProductInformation.SW_Revision = 0x05;

   TemperatureData.Adc.Size = 0;
   TemperatureData.Adc.WritePointer = 0;

   return Status;
}

STATUS EncodeData(T_S_PAGE Page, uint8_t *pData, uint8_t DataLength)
{
   STATUS Status = STATUS_SUCCESS;
   TEMPERATURE_DATA_SWITCH SwithTmp = TemperatureData.TemperatureSwitch;

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
         pData[3] = TemperatureData.Temperature[SwithTmp].Low24h & BYTE_MASK;
         pData[4] = (TemperatureData.Temperature[SwithTmp].Low24h >> 4) & UPPER_NIBBLE_MASK;
         pData[4] = pData[4] | (TemperatureData.Temperature[SwithTmp].High24h & LOWER_NIBBLE_MASK);
         pData[5] = (TemperatureData.Temperature[SwithTmp].High24h >> 4) & BYTE_MASK;
         pData[6] = TemperatureData.Temperature[SwithTmp].Current & BYTE_MASK;
         pData[7] = (TemperatureData.Temperature[SwithTmp].Current >> 8) & BYTE_MASK;
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

void UpdateCurrentTemperature(int16_t Temperature)
{
   switch (TemperatureData.TemperatureSwitch)
   {
      case SWITCH_0:
         TemperatureData.Temperature[SWITCH_1].Current = Temperature;
         break;
      case SWITCH_1:
         TemperatureData.Temperature[SWITCH_0].Current = Temperature;
         break;
   }
}

void UpdateMaxMinTemperature(int16_t Temperature)
{
   int16_t TemperatureHigh = TemperatureData.Temperature[TemperatureData.TemperatureSwitch].High24h;
   int16_t TemperatureLow = TemperatureData.Temperature[TemperatureData.TemperatureSwitch].Low24h;

   if((TemperatureHigh == INVALID_24H_TEMPERATURE) || (TemperatureHigh < Temperature))
   {
      TemperatureHigh = Temperature;
   }

   if((TemperatureLow == INVALID_24H_TEMPERATURE) || (TemperatureLow > Temperature))
   {
      TemperatureLow = Temperature;
   }

   switch (TemperatureData.TemperatureSwitch)
   {
      case SWITCH_0:
         TemperatureData.Temperature[SWITCH_1].High24h = TemperatureHigh;
         TemperatureData.Temperature[SWITCH_1].Low24h = TemperatureLow;
         break;
      case SWITCH_1:
         TemperatureData.Temperature[SWITCH_0].High24h = TemperatureHigh;
         TemperatureData.Temperature[SWITCH_0].Low24h = TemperatureLow;
         break;
   }
}

void UpdateTemperatureSwitch()
{
   switch (TemperatureData.TemperatureSwitch)
   {
      case SWITCH_0:
         TemperatureData.TemperatureSwitch = SWITCH_1;
         break;
      case SWITCH_1:
         TemperatureData.TemperatureSwitch = SWITCH_0;
         break;
   }
}

void BubbleSort(ADC_RESPONSE *Buffer, uint8_t BufferSize)
{
   do
   {
      for (uint8_t i = 0; i < BufferSize - 1; i++)
      {
         if (Buffer[i] > Buffer[i + 1])
         {
            ADC_RESPONSE temp = Buffer[i + 1];
            Buffer[i + 1] = Buffer[i];
            Buffer[i] = temp;
         }
      }
      BufferSize--;
   } while (BufferSize > 1);
}

ADC_RESPONSE GetMedian(ADC_RESPONSE *Buffer, uint8_t BufferSize)
{
   ADC_RESPONSE Median = 0;

   BubbleSort(Buffer, BufferSize);

   if (BufferSize & 1)
   {
      uint8_t BufferCenter = BufferSize / 2;

      Median = Buffer[BufferCenter];
   }
   else
   {
      uint8_t BufferCenter = BufferSize / 2;

      Median = (Buffer[BufferCenter] + Buffer[BufferCenter - 1]) / 2;
   }

   return Median;
}

void UpdateAdcData(ADC_RESPONSE Adc)
{
   TemperatureData.Adc.Data[TemperatureData.Adc.WritePointer] = Adc;

   if (TemperatureData.Adc.Size < ADC_BUFFER_SIZE)
   {
      TemperatureData.Adc.Size++;
   }

   TemperatureData.Adc.WritePointer++;
   if (TemperatureData.Adc.WritePointer >= ADC_BUFFER_SIZE)
   {
      TemperatureData.Adc.WritePointer = 0;
   }
}

ADC_RESPONSE ReadAdcData()
{
   STATUS Status = STATUS_FAILURE;
   ADC_RESPONSE AdcValue;
   Status = ADC_Read(IN 8, OUT &AdcValue);

   if (Status == STATUS_SUCCESS)
   {
      UpdateAdcData(AdcValue);
   }

   return GetMedian(TemperatureData.Adc.Data, TemperatureData.Adc.Size);
}

//Funckja s³u¿y do odczytu aktualnej temperatury
STATUS UpdateTemperature()
{
   STATUS Status = STATUS_FAILURE;
   char Message[30];
   ADC_RESPONSE AdcValue;
   float AdcValueVoltage = 0;
   int16_t TemperatureValue = 0;

   //Odczytanie wartoœci a przetwornika analogowo cyfrowego
   AdcValue = ReadAdcData();

   //Zamiana wartoœci bitowej na wartoœæ napiêcia
   Status = ADC_ValueToVoltage(AdcValue, &AdcValueVoltage);

   //Przeliczenie napiêcia na odpowiadaj¹ca temperaturê
   //i pomno¿enie tej wartoœæ razy 100
   TemperatureValue = ((AdcValueVoltage * 100) - 273) * 100;

   //Zapisanie wartoœci do odpowiednich rejestrów aplikacji
   UpdateCurrentTemperature(TemperatureValue);
   UpdateMaxMinTemperature(TemperatureValue / 10);
   UpdateTemperatureSwitch();

   if (Status == STATUS_SUCCESS)
   {
      sprintf(Message, "ADC = %d", AdcValue);
      DMsgMessageNewLine(IN strlen(Message), IN Message);
      sprintf(Message, "ADC [V] = %f", AdcValueVoltage);
      DMsgMessageNewLine(IN strlen(Message), IN Message);
      sprintf(Message, "Temp [C*100] = %d", TemperatureValue);
      DMsgMessageNewLine(IN strlen(Message), IN Message);
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

   UpdateTemperature();

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