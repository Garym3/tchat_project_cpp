#ifndef _server_h_
#define _server_h_

#ifndef __WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>

#include "Thread.h"
#include "Client.h"

#define PORT 30666

using namespace std;

class Server {

public:
	Server();
	void AcceptAndDispatch();
	static void * HandleClient(void *args);

private:
	static vector<Client> clients;

	int serverSocket{};
	struct sockaddr_in serverAddress{}, clientAddress{};

	static void ListClients();
	static void SendToAll(char *message, int senderClientId);
	static int FindClientId(Client *client);
};

#endif