#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>

#include "Global.h"
#include "Server.h"

using namespace std;
using namespace global;

class History
{
public:
	History();
	~History();
	
	static void read_history_and_send(const string& filePath, int clientSocket, int numberOfLines);
	static void append_to_history(const string& filePath, const string& message);
};

