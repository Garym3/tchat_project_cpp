#pragma once
#include <string>

using namespace std;

namespace global
{
#ifdef _WIN32
	const string newLine = "\r\n";
#else
	const string newLine = "\n";
#endif
}
