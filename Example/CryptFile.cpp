#include "stdafx.h"
#include "CryptFile.h"


namespace Example
{

	void CryptFile::Crypt(void)
	{

		this->OpenFileSource().OpenFileDestanition().CreateEvents().Work(&ICrypt::Crypt).Wait();

	}

	void CryptFile::Decrypt(void)
	{

		this->OpenFileSource().OpenFileDestanition().CreateEvents().Work(&ICrypt::Decrypt).Wait();

	}

	CryptFile & CryptFile::OpenFileSource(void)
	{

		this->ifs.open(this->wsPath,
			std::ios::binary);
		if (!this->ifs)
			throw std::runtime_error("Error open file source");

		return *this;

	}

	CryptFile & CryptFile::OpenFileDestanition(void)
	{

		this->ofs.open(wsTempFileName,
			std::ios::binary);
		if (!this->ofs)
			throw std::runtime_error("Error open file destanition");

		return *this;

	}

	CryptFile & CryptFile::CreateEvents(void)
	{

		this->meExit = ManualEvent::Create();

		this->vecEvents.resize(IndexEvents::Count);

		for (auto i = 0; i < IndexEvents::Count; ++i)
			this->vecEvents[i] = AutoEvent::Create();

		return *this;

	}

	CryptFile & CryptFile::Work(void(ICrypt::*methodPtr)(std::vector<BYTE>&, std::size_t))
	{

		this->vecThreads.push_back(std::thread{ &CryptFile::ThreadRead,
			this });

		this->vecThreads.push_back(std::thread{ &CryptFile::ThreadCryptDecrypt,
			this,
			methodPtr });

		this->vecThreads.push_back(std::thread{ &CryptFile::ThreadWrite,
			this });

		return *this;

	}

	void CryptFile::ThreadRead(void)
	{

		try
		{

			std::vector<HANDLE> vecEvents
			{

				this->meExit,
				this->vecEvents[IndexEvents::Read]

			};

			while (!this->ifs.eof())
			{

				this->vecBufRead = this->ReadFromFile();

				this->vecEvents[IndexEvents::CryptDecrypt].Set();

				switch (Handle::Wait(vecEvents, FALSE))
				{

				case IndexWaits::OnExit:
					return;

				}

			}

			this->vecEvents[IndexEvents::ReadEnd].Set();

		}
		catch (const std::exception& ex)
		{

			std::wcout << Convert::AnsiToUnicode(ex.what()) << std::endl;

			this->meExit.Set();

		}

	}

	void CryptFile::ThreadCryptDecrypt(void(ICrypt::*methodPtr)(std::vector<BYTE>&, std::size_t))
	{

		try
		{

			std::vector<HANDLE> vecEvents
			{

				this->meExit,
				this->vecEvents[IndexEvents::CryptDecrypt],
				this->vecEvents[IndexEvents::ReadEnd],
				this->vecEvents[IndexEvents::WriteOk]

			};

			bool bFlag = true;

			while (bFlag)
			{

				switch (Handle::Wait(vecEvents, FALSE))
				{

				case IndexWaits::OnExit:
					return;

				case IndexWaits::OnWriteOk:
					continue;

				case IndexWaits::OnCryptDecrypt:
				{

					auto vecBuf = std::move(this->vecBufRead);

					this->vecEvents[IndexEvents::Read].Set();

					(this->uPtrCrypt.get()->*methodPtr)(vecBuf,
						vecBuf.size());

					this->vecBufWrite = std::move(vecBuf);

					this->vecEvents[IndexEvents::Write].Set();

				}
				break;

				default:
					bFlag = false;

				}

			}

			this->vecEvents[IndexEvents::CryptDecryptEnd].Set();

		}
		catch (const std::exception& ex)
		{

			std::wcout << Convert::AnsiToUnicode(ex.what()) << std::endl;

			this->meExit.Set();

		}

	}

	void CryptFile::ThreadWrite(void)
	{

		try
		{

			std::vector<HANDLE> vecEvents
			{

				this->meExit,
				this->vecEvents[IndexEvents::Write],
				this->vecEvents[IndexEvents::CryptDecryptEnd]

			};

			bool bFlag = true;

			while (bFlag)
			{

				switch (Handle::Wait(vecEvents, FALSE))
				{

				case IndexWaits::OnExit:
					return;

				case IndexWaits::OnWrite:
				{

					auto vecBuf = std::move(this->vecBufWrite);

					this->vecEvents[IndexEvents::WriteOk].Set();

					this->WriteToFile(vecBuf);

				}

				break;

				default:
					bFlag = false;

				}

			}

			this->FileReplace();

		}
		catch (const std::exception& ex)
		{

			std::wcout << Convert::AnsiToUnicode(ex.what()) << std::endl;

			this->meExit.Set();

		}

	}

	void CryptFile::Wait(void)
	{

		for (auto & i : this->vecThreads)
			i.join();

	}

	std::vector<BYTE> CryptFile::ReadFromFile(void)
	{

		std::vector<BYTE> vecBuf(CryptFile::sizeBuf);

		auto sizeRead = this->ifs.read(reinterpret_cast<char*>(&vecBuf[0]),
			vecBuf.size()).gcount();

		if (sizeRead && sizeRead != CryptFile::sizeBuf)
			vecBuf.resize(static_cast<std::size_t>(sizeRead));

		return vecBuf;

	}

	void CryptFile::WriteToFile(const std::vector<BYTE>& vecBuf)
	{

		if (this->ofs.write(reinterpret_cast<const char*>(vecBuf.data()),
			vecBuf.size()).fail())
			throw std::runtime_error("Error write to file");

	}

	void CryptFile::FileReplace(void)
	{

		this->ifs.close();

		this->ofs.close();

		File::Copy(wsTempFileName,
			this->wsPath);

		File::Delete(wsTempFileName);

	}

}