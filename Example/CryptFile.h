#pragma once

#include "ICrypt.h"
#include "Event.h"
#include "Convert.h"
#include "File.h"

namespace Example
{

	class CryptFile
	{

		std::wstring wsPath;

		std::unique_ptr<ICrypt> uPtrCrypt;

		std::ifstream ifs;

		std::ofstream ofs;

		std::vector<BYTE> vecBufRead, vecBufWrite;

		ManualEvent meExit;

		std::vector<std::thread> vecThreads;

		static const std::size_t sizeBuf = std::size_t(4096);

		enum IndexEvents : std::size_t {
			Read,
			CryptDecrypt,
			ReadEnd,
			CryptDecryptEnd,
			Write,
			WriteOk,
			Count = 6
		};

		std::vector<AutoEvent> vecEvents;

		enum IndexWaits : DWORD {
			OnExit,
			OnCryptDecrypt,
			OnRead = OnCryptDecrypt,
			OnEndRead,
			OnWrite = OnCryptDecrypt,
			OnEndCryptDecrypt,
			OnWriteOk
		};

		CryptFile(const CryptFile&) = delete;
		CryptFile & operator = (const CryptFile&) = delete;
		CryptFile(CryptFile&&) = delete;
		CryptFile & operator = (CryptFile&&) = delete;

	public:

		CryptFile(void) = default;

		~CryptFile(void) = default;

		CryptFile & SetPath(const std::wstring& wsPath)
		{

			this->wsPath = wsPath;

			return *this;

		}

		CryptFile & SetCrypt(const std::unique_ptr<ICrypt>& uPtrCrypt)
		{

			this->uPtrCrypt = uPtrCrypt->Clone();

			return *this;

		}

		void Crypt(void);

		void Decrypt(void);

		void Cancel(void) const
		{

			this->meExit.Set();

		}

	private:

		CryptFile & OpenFileSource(void);

		CryptFile & OpenFileDestanition(void);

		CryptFile & CreateEvents(void);

		CryptFile & Work(void(ICrypt::*)(std::vector<BYTE>&, std::size_t));

		void ThreadRead(void);

		void ThreadCryptDecrypt(void(ICrypt::*)(std::vector<BYTE>&, std::size_t));

		void ThreadWrite(void);

		void Wait(void);

		std::vector<BYTE> ReadFromFile(void);

		void WriteToFile(const std::vector<BYTE>&);

		void FileReplace(void);

	};

}