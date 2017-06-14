extern "C" {
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/typedef.h"
#include "../../Atmega2560_Rs232/Atmega2560_Rs232/FIFO/Fifo.h"
}
#using <System.dll>

#include "UART.h"

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

FIFO gUART0FifoIn;
FIFO gUART0FifoOut;

STATUS USART_WriteToFifo(IN UART_ENUM Uart, IN unsigned char data)
{
	return WriteToFifo(INOUT &gUART0FifoIn, IN data);
}

public ref class PortChat
{
private:
	static bool _continue;
	static SerialPort^ _serialPort;
	static Thread^ _readThread;
	static Thread^ _writeThread;
	static bool _working;

public:
	static void INIT(uint32_t baud)
	{
		String^ name;
		String^ message;
		StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		_readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));
		_writeThread = gcnew Thread(gcnew ThreadStart(PortChat::Write));

		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort();

		// Allow the user to set the appropriate properties.
		_serialPort->PortName = SetPortName(_serialPort->PortName);
		_serialPort->BaudRate = baud; // SetPortBaudRate(_serialPort->BaudRate);
		_serialPort->Parity = Parity::None;//SetPortParity(_serialPort->Parity);
		_serialPort->DataBits = 8; // SetPortDataBits(_serialPort->DataBits);
		_serialPort->StopBits = StopBits::One; // SetPortStopBits(_serialPort->StopBits);
		_serialPort->Handshake = Handshake::None; // SetPortHandshake(_serialPort->Handshake);

												  // Set the read/write timeouts
		_serialPort->ReadTimeout = 500;
		_serialPort->WriteTimeout = 500;

		_serialPort->Open();
		_continue = true;
		_readThread->Start();
		_writeThread->Start();
		_working = true;
		Suspend();

		/*while (_continue)
		{
			message = Console::ReadLine();

			if (stringComparer->Equals("quit", message))
			{
				_continue = false;
			}
			else
			{
				//_serialPort->WriteLine(String::Format("<{0}>: {1}", name, message));
				array<unsigned char>^ _buffer = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };
				_serialPort->Write(_buffer, 0, 7);
			}
		}*/		
	}

	static String^ SetPortName(String^ defaultPortName)
	{
		String^ portName;

		Console::WriteLine("Available Ports:");
		for each (String^ s in SerialPort::GetPortNames())
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Enter COM port value (Default: {0}): ", defaultPortName);
		portName = Console::ReadLine();

		if (portName == "")
		{
			portName = defaultPortName;
		}
		return portName;
	}

	static void Read()
	{
		while (_continue)
		{
			try
			{
				//array<unsigned char>^ _buffer = { 0xA4, 0x01, 0x4A, 0x00, 0xEF, 0x00, 0x00 };
				//int a = _serialPort->Read(_buffer, 0, 7);
				int a = _serialPort->ReadByte();
				USART_WriteToFifo(UART0, (unsigned char)(a));
				if (a > 0)
				{
					//Console::Write(a);
					//Console::Write("Hex: {0:X}", a);
				}
				else
				{
					//Console::Write("\n", a);
				}

			}
			catch (TimeoutException ^) {}
		}
	}

	static void Write()
	{
		while (_continue)
		{
			unsigned char data;
			STATUS Status;
			
			Status = ReadFromFifo(INOUT &gUART0FifoOut, OUT &data);

			if (STATUS_SUCCESS == Status)
			{
				//REGISTER_ADRESS_8(UartAddress.UDR) = data;
				array<unsigned char>^ _buffer = { 0x00 };
				_buffer[0] = data;
				try
				{
					_serialPort->Write(_buffer, 0, _buffer->Length);
				}
				catch (TimeoutException ^) {}
			}
			else
			{
				//REGISTER_ADRESS_8(UartAddress.UCSRB) &= ~(1 << UDRIE3); //wy³¹cz przerwania pustego bufora nadawania
				Suspend();
			}			
		}
	}

	static void ClosePort()
	{
		Console::ReadLine();
		_continue = false;

		_readThread->Join();
		_serialPort->Close();
	}

	static void Resume()
	{
		/*if (_working == false)
		{
			_writeThread->Resume();
			_working = true;
		}*/
	}

	static void Suspend()
	{
		_writeThread->Sleep(10);
		/*if (_working == true)
		{
			_writeThread->Suspend();
			_working = false;
		}*/
	}
};

void USART_Init(uint32_t baud, UART_ENUM uart)
{
	FifoInit(OUT &gUART0FifoIn);
	FifoInit(OUT &gUART0FifoOut);

	PortChat::INIT(baud);
}

void USART_Close(UART_ENUM uart)
{
	PortChat::ClosePort();
}

STATUS USART_ReadByteFromFifo(IN UART_ENUM Uart, OUT unsigned char *pData)
{
	return ReadFromFifo(INOUT &gUART0FifoIn, IN pData);
}

STATUS USART_TransmitByteByFifo(IN UART_ENUM Uart, IN unsigned char data)
{
	if (STATUS_SUCCESS == WriteToFifo(INOUT &gUART0FifoOut, IN data))
	{
		//REGISTER_ADRESS_8(UartAddress.UCSRB) |= (1 << UDRIE3);
		PortChat::Resume();

		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_FAILURE;
	}
}

extern "C" STATUS USART_Transmit(IN UART_ENUM Uart, IN UART_LENGTH UartLength, IN unsigned char *pUartData)
{
	int i;
	STATUS Status;

	for (i = 0; i < UartLength; i++)
	{
		Status = WriteToFifo(&gUART0FifoOut, IN pUartData[i]);

		if (STATUS_SUCCESS == Status)
		{

		}
		else
		{
			return Status;
		}
	}

	//REGISTER_ADRESS_8(UartAddress.UCSRB) |= (1 << UDRIE3);
	PortChat::Resume();

	return Status;
}