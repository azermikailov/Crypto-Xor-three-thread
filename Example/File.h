#pragma once
#include "WinapiException.h"

namespace Example
{

	class File
	{

	public:

		File(void) = default;

		~File(void) = default;

		static DWORD GetAttributes(const std::wstring&);

		static void Copy(const std::wstring&,
			const std::wstring&,
			BOOL = TRUE);

		static void Delete(const std::wstring&);

	};

}

