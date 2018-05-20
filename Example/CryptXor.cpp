#include "stdafx.h"
#include "CryptXor.h"

namespace Example
{

	void CryptXor::Crypt(std::vector<BYTE>& vecBuf,
		std::size_t sizeBuf)
	{

		sizeBuf = sizeBuf == ICrypt::nSize ? vecBuf.size() : sizeBuf;

		for (auto i = std::size_t(0); i < sizeBuf; ++i)
			vecBuf[i] ^= ICrypt::vecKey[i % ICrypt::vecKey.size()];

	}

	void CryptXor::Decrypt(std::vector<BYTE>& vecBuf,
		std::size_t sizeBuf)
	{

		this->Crypt(vecBuf,
			sizeBuf);

	}

}