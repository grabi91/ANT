extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/Debug_MSG/Debug_Msg.h"
}

#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

int main(int argc, char **argv)
{
	DMsgMessageNewPage(IN 18, IN(unsigned char*)"Poczatek Programu.");
	DMsgMessageNewLine(IN 20, IN(unsigned char*)"Druga Linia Programu");
	DMsgMessage(IN 14, IN(unsigned char*)". Dalszy ciag.");
	DMsgMessageNewLine(IN 0, IN(unsigned char*)"");
	return 0;
}