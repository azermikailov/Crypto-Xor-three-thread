#include "stdafx.h"
#include "CommandLine.h"
#include"WinapiException.h"
namespace Example
{

	std::vector<std::wstring> CommandLine::GetCommandLineArguments(void)
	{

		auto iCount = 0;

		auto pArgList = ::CommandLineToArgvW(::GetCommandLineW(),
			&iCount);
		if (!pArgList)
			throw CommandLineException(L"Error CommandLineToArgvW",
				::GetLastError());

		std::vector<std::wstring> lstReturn;
		for (auto i = 0; i < iCount; lstReturn.push_back(pArgList[i++]));

		::LocalFree(pArgList);

		return lstReturn;

	}

}