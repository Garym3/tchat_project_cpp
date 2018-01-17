#ifndef __WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#endif

#include <iostream>
#include <minwindef.h>

#include "Thread.h"
#include "Server.h"

#ifndef __WIN32
#pragma comment(lib, "wsock32.lib")
#endif

using namespace std;

int main() {
#ifndef __WIN32
	const auto versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);
#endif

	cout << "Server is running!" << endl;

	Server server = Server{};

	// Main loop
	server.ServerDeployment();

#ifndef __WIN32
	WSACleanup();
#endif
	return 0;
}

