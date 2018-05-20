#pragma once


namespace Example
{

	class Convert
	{

		enum { FullLength = -1 };

	public:

		Convert(void) = default;
		~Convert(void) = default;

		static std::string Convert::UnicodeToAnsi(const std::wstring&,
			int iLen = FullLength);

		static std::wstring Convert::AnsiToUnicode(const std::string&);

		static std::vector<BYTE> StringToBytes(const std::wstring&);

	};

}