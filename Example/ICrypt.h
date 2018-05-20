#pragma once

namespace Example
{

	class ICrypt
	{

	protected:

		std::vector<BYTE> vecKey;

	public:

		static const std::size_t nSize = -1;

		ICrypt(void) = default;

		virtual ~ICrypt(void) = default;

		void SetKey(const std::vector<BYTE>& vecKey)
		{

			this->vecKey = vecKey;

		}

		const std::vector<BYTE> GetKey(void) const throw()
		{

			return this->vecKey;

		}

		virtual void Crypt(std::vector<BYTE>&,
			std::size_t = ICrypt::nSize) = 0;

		virtual void Decrypt(std::vector<BYTE>&,
			std::size_t = ICrypt::nSize) = 0;

		virtual std::unique_ptr<ICrypt> Clone(void) const = 0;

		template<typename T>
		static std::unique_ptr<ICrypt> Create(void)
		{

			return std::unique_ptr<ICrypt>(new T);

		}

	};

}