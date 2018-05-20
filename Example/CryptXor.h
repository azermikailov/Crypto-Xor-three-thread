#pragma once
#include "ICrypt.h"

namespace Example
{

	class CryptXor : public ICrypt
	{

	public:

		using ICrypt::ICrypt;

		void Crypt(std::vector<BYTE>&,
			std::size_t = ICrypt::nSize) override;

		void Decrypt(std::vector<BYTE>&,
			std::size_t = ICrypt::nSize) override;

		std::unique_ptr<ICrypt> Clone(void) const override
		{

			auto uPtrCopy = ICrypt::Create<CryptXor>();

			uPtrCopy->SetKey(this->GetKey());

			return uPtrCopy;

		}

	};

}