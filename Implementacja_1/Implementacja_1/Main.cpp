extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/Debug_MSG/Debug_Msg.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/ANT/ANT_Framer.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/ANT/ANT_Message_Queue.h"
}

#include "UART.h"
#include <stdio.h>

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

   DMsgMessageNewPage(IN 19, IN(unsigned char*)"Poczatek Programu.");
   DMsgMessageNewLine(IN 21, IN(unsigned char*)"Druga Linia Programu");
   DMsgMessage(IN 15, IN(unsigned char*)". Dalszy ciag.");
   DMsgMessageNewLine(IN 1, IN(unsigned char*)"");

   USART_Init(IN 57600, IN UART0);

   Status = ANT_Init();

   while (1)
   {
      Status = ANT_Framer_GetMessage(&AntMessage);
      if (STATUS_SUCCESS == Status)
      {
         Status = ANT_Mesg_Q_ProcessPayload(&AntMessage);

         if (Status != STATUS_SUCCESS)
         {
            DMsgMessageNewLine(5, (unsigned char*)"Blad");
         }
      }

   }

   USART_Close(IN UART0);

   return 0;
}