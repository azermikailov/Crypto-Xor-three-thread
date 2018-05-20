#pragma once

namespace Example
{

	class CommandLine
	{

	public:

		CommandLine(void) = default;
		~CommandLine(void) = default;

		static std::vector<std::wstring> GetCommandLineArguments(void);

	};

}