#pragma once

#include"stdafx.h"
#include "WinapiException.h"

namespace Example
{

	class Handle
	{

	protected:

		HANDLE h = nullptr;

		Handle(const Handle&) = delete;
		const Handle & operator = (const Handle&) = delete;

	public:

		explicit Handle(HANDLE h = nullptr) throw() : h(h) { }

		Handle(Handle&& hRight) throw() : h(hRight.Release()) { }

		Handle & operator = (Handle&& hRight) throw()
		{

			return this != &hRight ? this->Reset(hRight.Release()) : *this;

		}

		~Handle(void) throw()
		{

			this->Close();

		}

		void Close(void)throw()
		{

			if (this->h && this->h != INVALID_HANDLE_VALUE)
				::CloseHandle(this->h);

			this->h = nullptr;

		}

		Handle & Reset(HANDLE h = nullptr)throw()
		{

			this->Close();

			this->h = h;

			return *this;

		}

		operator HANDLE (void) const throw()
		{

			return this->h;

		}

		bool operator ! (void) const throw()
		{

			return this->h == nullptr || this->h == INVALID_HANDLE_VALUE;

		}

		DWORD Wait(DWORD dwTimeOut = INFINITE) const
		{

			auto dwReturn = ::WaitForSingleObject(this->h,
				dwTimeOut);
			if (dwReturn == WAIT_FAILED)
				throw EventException(L"Ошибка WaitForSingleObject",
					::GetLastError());

			return dwReturn;

		}

		static DWORD Wait(const std::vector<Handle>& vecHandles,
			BOOL bWaitAll = TRUE,
			DWORD dwTimeOut = INFINITE)
		{

			return Handle::Wait(std::vector<HANDLE>(vecHandles.cbegin(),
				vecHandles.cend()),
				bWaitAll,
				dwTimeOut);

		}

		static DWORD Wait(const std::vector<HANDLE>& vecHandles,
			BOOL bWaitAll = TRUE,
			DWORD dwTimeOut = INFINITE)
		{

			auto dwReturn = WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),
				vecHandles.data(),
				bWaitAll,
				dwTimeOut);

			if (dwReturn == WAIT_FAILED)
				throw EventException(L"Ошибка WaitForSingleObject",
					::GetLastError());

			return dwReturn;

		}

		HANDLE Release(void) throw()
		{

			HANDLE h = this->h;

			this->h = nullptr;

			return h;

		}

	};

}