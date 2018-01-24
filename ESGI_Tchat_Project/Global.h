#pragma once
#include <string>

using namespace std;

namespace global
{
#ifndef __WIN32
	const string newLine = "\r\n";
#else
	const string newLine = "\n";
#endif
}
