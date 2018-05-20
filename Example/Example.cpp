// Example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinapiException.h"
#include "Convert.h"
#include "CommandLine.h"
#include "CryptFile.h"
#include "CryptXor.h"
#include "File.h"

namespace Example
{

	AutoEvent evFinish;

	std::thread th;

	enum : DWORD { WaitTimeOut = 1 };

	CryptFile cf;

	void Thread(void);

}


int main()
{

	_wsetlocale(LC_ALL, L".866");

	try
	{

		Example::evFinish = Example::AutoEvent::Create();

		Example::th = std::thread{ Example::Thread };

		while (Example::evFinish.Wait(Example::WaitTimeOut))
		{

			if (_kbhit())
			{

				Example::cf.Cancel();

				std::wcout << std::endl << L"Canceled" << std::endl;

				break;

			}

		}

		Example::th.join();

	}
	catch (const std::exception& ex)
	{

		std::wcout << Example::Convert::AnsiToUnicode(ex.what()) << std::endl;

	}
	catch (...)
	{

		std::wcout << L"Unhandled exception in the main" << std::endl;

	}

	return 0;

}

void Example::Thread(void)
{

	try
	{

		auto lstArgs = Example::CommandLine::GetCommandLineArguments();
		if (lstArgs.size() != Example::CountArgs || !Example::File::GetAttributes(lstArgs[IndexCommandLine::Path]) || !lstArgs[IndexCommandLine::Key].length() || (lstArgs[IndexCommandLine::Mode] != L"c" && lstArgs[IndexCommandLine::Mode] != L"d"))
			throw Example::CommandLineException(L"Invalid arguments");

		auto uPtrCrypt = Example::ICrypt::Create<Example::CryptXor>();
		uPtrCrypt->SetKey(Example::Convert::StringToBytes(lstArgs[IndexCommandLine::Key]));

		auto timeStart = time(nullptr);

		Example::cf.SetPath(lstArgs[IndexCommandLine::Path]).SetCrypt(uPtrCrypt);

		lstArgs[IndexCommandLine::Mode] == L"c" ? Example::cf.Crypt() : Example::cf.Decrypt();

		std::wcout << "Working time = " << time(nullptr) - timeStart << std::endl;

	}
	catch (const std::exception& ex)
	{

		std::wcout << Example::Convert::AnsiToUnicode(ex.what()) << std::endl;

	}
	catch (...)
	{

		std::wcout << L"Unhandled exception in the Thread" << std::endl;

	}

	Example::evFinish.Set();

}
