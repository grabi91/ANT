extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/Debug_MSG/Debug_Msg.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/ANT/ANT_Framer.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/ANT/ANT_Message_Queue.h"
}

#include "UART.h"
#include <stdio.h>

#define USER_BAUDRATE         (50000)  // For AT3/AP2, use 57600
#define USER_RADIOFREQ        (57)

#define USER_ANTCHANNEL       (0)
#define USER_DEVICENUM        (100)
#define USER_DEVICETYPE       (25)
#define USER_TRANSTYPE        (5)

#define USER_NETWORK_KEY      {0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45}
#define USER_NETWORK_KEY_SIZE (8)
#define USER_NETWORK_NUM      (0)      // The network key is assigned to this network number

STATUS ANT_Init()
{
   STATUS Status = STATUS_SUCCESS;
   unsigned char NetKey[USER_NETWORK_KEY_SIZE] = USER_NETWORK_KEY;

   Status = ANT_Mesg_Q_Init();

   if (Status == STATUS_SUCCESS)
   {
      Status = ANT_Framer_Mesg_ResetSystem();
   }

   Status = ANT_Framer_Mesg_SetNetworkKey(USER_NETWORK_NUM, NetKey, USER_NETWORK_KEY_SIZE);

   return Status;
}

int main(int argc, char **argv)
{
   int a = 0;
   STATUS Status;
   ANT_MESSAGE_ITEM AntMessage;
   unsigned char dataByte = 0;

   DMsgMessageNewPage(IN 18, IN(unsigned char*)"Poczatek Programu.");
   DMsgMessageNewLine(IN 20, IN(unsigned char*)"Druga Linia Programu");
   DMsgMessage(IN 14, IN(unsigned char*)". Dalszy ciag.");
   DMsgMessageNewLine(IN 0, IN(unsigned char*)"");

   USART_Init(IN 57600, IN UART0);

   Status = ANT_Init();

   while (1)
   {
      Status = ANT_Framer_GetMessage(&AntMessage);
      if (STATUS_SUCCESS == Status)
      {
         Status = ANT_Mesg_Q_ProcessPayload(&AntMessage);
      }

   }

   USART_Close(IN UART0);

   return 0;
}