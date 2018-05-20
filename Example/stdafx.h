// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <vector>
#include <exception>
#include <Windows.h>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <fstream>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <conio.h>

namespace Example
{

	const std::size_t CountArgs = 4;

	const std::wstring wsTempFileName(L"Crypt.tmp");

	enum IndexCommandLine : unsigned char { Path = 1, Key, Mode };

}


// TODO: reference additional headers your program requires here
