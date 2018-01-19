#pragma once
#include <string>

using namespace std;

namespace global
{
#ifndef __WIN32
	string newLine = "\r\n";
#else
	string newLine = "\n";
#endif
}
