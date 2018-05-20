#pragma once


namespace Example
{

	class WinapiException : public std::exception
	{

	protected:

		mutable std::wstring wsText;
		mutable std::string sText;
		DWORD dwCode;

	public:

		using std::exception::exception;

		WinapiException(const std::wstring& wsText = std::wstring(),
			DWORD dwCode = 0) : wsText(wsText), dwCode(dwCode) { }

		const char * WinapiException::what(void)const override;

		virtual std::wstring What(void) const;

		DWORD WinapiException::Code(void) const throw()
		{

			return this->dwCode;

		}

	};

	class ConvertException : public WinapiException
	{

	public:

		using WinapiException::WinapiException;

	};

	class CommandLineException : public WinapiException
	{

	public:

		using WinapiException::WinapiException;

	};

	class EventException : public WinapiException
	{

	public:

		using WinapiException::WinapiException;

	};

	class FileException : public WinapiException
	{

	public:

		using WinapiException::WinapiException;

	};


}
