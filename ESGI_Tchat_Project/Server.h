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
#include <iostream>
#include <fstream>

#include "Thread.h"
#include "Client.h"

#define PORT 30666

using namespace std;

class Server {

public:
	Server();
	void serverDeployment();
	static void * handleClient(void *args);

private:
	static vector<Client> clients;

	int serverSocket{};
	struct sockaddr_in serverAddress{}, clientAddress{};

	static int sendTo(const Client* client, const string& message);
	static void receive(const Client* client, const string& message);
	static void listClients();
	static void sendToAll(char *message, int senderClientId);
	static int findClientId(Client *client);
	static void readFromHistoryAndSend(const string& filePath, const Client* client);
	static void writeInHistory(const string& filePath, const string& message);
};

#endif