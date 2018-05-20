#include "stdafx.h"
#include "Convert.h"
#include "WinapiException.h"

namespace Example
{

	std::string Convert::UnicodeToAnsi(const std::wstring &wsParam,
		int iLen)
	{

		std::string strReturn(iLen == FullLength ? wsParam.size() + 1 : iLen + 1, '\0');

		int iRet = ::WideCharToMultiByte(CP_ACP,
			WC_NO_BEST_FIT_CHARS,
			wsParam.c_str(),
			iLen,
			&strReturn[0],
			static_cast<int>(strReturn.size()),
			0,
			0);
		if (!iRet)
			throw ConvertException(L"Ошибка WideCharToMultiByte",
				::GetLastError());

		return strReturn.c_str();

	}

	std::wstring Convert::AnsiToUnicode(const std::string &sParam)
	{

		std::wstring wsReturn(sParam.size() + 1, '\0');

		int iRet = ::MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			sParam.c_str(),
			sParam.size(),
			&wsReturn[0],
			static_cast<int>(wsReturn.size()));
		if (!iRet)
			throw ConvertException(L"Ошибка MultiByteToWideChar",
				::GetLastError());

		return wsReturn.c_str();

	}

	std::vector<BYTE> Convert::StringToBytes(const std::wstring& wsParam)
	{

		return std::vector<BYTE>(reinterpret_cast<LPCBYTE>(&wsParam[0]),
			reinterpret_cast<LPCBYTE>(&wsParam[0]) + wsParam.length() * sizeof(wchar_t));

	}

}