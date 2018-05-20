#include "stdafx.h"
#include "File.h"



namespace Example
{

	DWORD File::GetAttributes(const std::wstring& wsPath)
	{

		auto dwResult = GetFileAttributes(wsPath.c_str());
		if (dwResult == INVALID_FILE_ATTRIBUTES)
			throw FileException(L"Error GetFileAttributes",
				GetLastError());

		return dwResult;

	}

	void File::Copy(const std::wstring& wsPathFrom,
		const std::wstring& wsPathTo,
		BOOL bOverride)
	{

		if (!CopyFile(wsPathFrom.c_str(),
			wsPathTo.c_str(),
			bOverride))
			throw FileException(L"Error CopyFile",
				GetLastError());

	}

	void File::Delete(const std::wstring& wsPath)
	{

		if (!DeleteFile(wsPath.c_str()))
			throw FileException(L"Error DeleteFile",
				GetLastError());

	}

}