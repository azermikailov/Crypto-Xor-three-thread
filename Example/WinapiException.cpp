#include "stdafx.h"
#include "WinapiException.h"
#include "Convert.h"


namespace Example
{

	const char * WinapiException::what(void) const
	{

		return (this->sText = Convert::UnicodeToAnsi(this->What())).c_str();

	}

	std::wstring WinapiException::What(void) const
	{
		LPVOID lpMsgBuf = nullptr;

		std::wostringstream stream;

		stream << this->wsText << std::endl;

		if (this->dwCode)
		{

			if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				this->dwCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPWSTR>(&lpMsgBuf),
				0,
				0))
			{

				stream << reinterpret_cast<LPCWSTR>(lpMsgBuf) << std::endl;
				::LocalFree(lpMsgBuf);

			}

		}

		return stream.str();

	}


}