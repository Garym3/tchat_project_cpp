#ifndef __WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#endif

#include <minwindef.h>

#include "Server.h"
#include "Global.h"

#ifndef __WIN32
#pragma comment(lib, "wsock32.lib")
#endif

using namespace global;

int main() {
#ifndef __WIN32
	const auto versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);
#endif

	printf("Server is running!%s", newLine.c_str());

	Server server = Server{};

	// Main loop
	server.serverDeployment();

#ifndef __WIN32
	WSACleanup();
#endif
	return 0;
}

