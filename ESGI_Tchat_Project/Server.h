#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <vector>

#include "Thread.h"
#include "Client.h"
#include "Global.h"
#include "History.h"

#define PORT 30666

using namespace std;
using namespace global;

class History;

class Server {

public:
	Server();
	void server_deployment();
	static void * handle_client(void *args);
	static vector<Client> get_server_clients();
	static int send_to(const string& message, int clientSocket);

private:
	static vector<Client> clients;

	int serverSocket{};
	struct sockaddr_in serverAddress{}, clientAddress{};

	static void send_to_all(const string& message, int senderClientId);
	static void shutdown_client(int clientSocket);
	static bool handle_data(Client* client, char* message, bool isFirstMessage);
	static int find_client_id(Client *client);
	static void handle_commands(Client* client, const string& message);
};