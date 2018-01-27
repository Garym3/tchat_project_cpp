#pragma once

#ifndef __WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>
//#include <chrono>

#include "Thread.h"
#include "Client.h"
#include "Global.h"

#define PORT 30666

using namespace std;

//typedef std::chrono::high_resolution_clock Clock;

class Server {

public:
	Server();
	void serverDeployment();
	static void * handleClient(void *args);

private:
	static vector<Client> clients;

	int serverSocket{};
	struct sockaddr_in serverAddress{}, clientAddress{};

	static int sendTo(const string& message, int clientSocket);
	static void receive(const Client* client, const string& message);
	static void sendToAll(const string& message, int senderClientId = -1);
	static void shutdownClient(int clientSocket);
	static bool handleData(const Client* client, char* message, bool isFirstMessage);
	static int findClientId(Client *client);
	static void readHistoryAndSend(const string& filePath, int clientSocket, int numberOfLines);
	static void appendToHistory(const string& filePath, const string& message);
};