#pragma once
#include"stdafx.h"
#include "WrapHandle.h"

namespace Example
{

	class IEvent
	{

	protected:

		Handle h;

		IEvent(const IEvent&) = delete;
		const IEvent & operator = (const IEvent&) = delete;

	public:

		explicit IEvent(HANDLE h = nullptr) : h(h) { }

		explicit IEvent(Handle&& hRight) : IEvent(hRight.Release()) { }

		explicit IEvent(IEvent&& eRight) : IEvent(eRight.h) { }

		IEvent & operator = (IEvent&& eRight)
		{

			if (this != &eRight)
				this->h.Reset(eRight.h.Release());

			return *this;

		}

		virtual ~IEvent(void) = default;

		void Set(void) const
		{

			if (!::SetEvent(this->h))
				throw EventException(L"Ошибка SetEvent",
					::GetLastError());

		}

		virtual void Reset(void) const = 0;

		void Close(void)
		{

			this->h.Close();

		}

		DWORD Wait(DWORD dwTimeOut = INFINITE) const
		{

			return this->h.Wait(dwTimeOut);

		}

		operator HANDLE (void) const throw()
		{

			return this->h;

		}

	protected:

		static Handle Create(LPSECURITY_ATTRIBUTES lpSecAttr = nullptr,
			BOOL bManualReset = FALSE,
			BOOL bInitialState = FALSE,
			const std::wstring& wsName = std::wstring())
		{

			Handle h(::CreateEvent(lpSecAttr,
				bManualReset,
				bInitialState,
				wsName.c_str()));
			if (!h)
				throw EventException(L"Ошибка CreateEvent",
					::GetLastError());

			return h;

		}

	};

	class ManualEvent : public IEvent
	{

	public:

		using IEvent::IEvent;

		void Reset(void) const override
		{

			if (!::ResetEvent(IEvent::h))
				throw EventException(L"Ошибка ResetEvent",
					::GetLastError());

		}

		static ManualEvent Create(LPSECURITY_ATTRIBUTES lpSecAttr = nullptr,
			BOOL bInitialState = FALSE,
			const std::wstring& wsName = std::wstring())
		{

			return ManualEvent(IEvent::Create(lpSecAttr,
				TRUE,
				bInitialState,
				wsName));

		}

	};

	class AutoEvent : public IEvent
	{

	public:

		using IEvent::IEvent;

		void Reset(void) const override
		{

			throw std::exception("Not supported");

		}

		static AutoEvent Create(LPSECURITY_ATTRIBUTES lpSecAttr = nullptr,
			BOOL bInitialState = FALSE,
			const std::wstring& wsName = std::wstring())
		{

			return AutoEvent(IEvent::Create(lpSecAttr,
				FALSE,
				bInitialState,
				wsName));

		}

	};

}