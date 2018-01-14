#include "Server.h"

using namespace std;

//Actually allocate clients
vector<Client> Server::clients;

Server::Server() {

	//Initialize static mutex from Thread
	Thread::InitMutex();

	//For setsock opt (REUSEADDR)
	char yes = 1;

	//Init serverSock and start listen()'ing
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	//Avoid bind error if the socket was not close()'d last time;
	setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	if (bind(serverSock, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(sockaddr_in)) < 0)
		cerr << "Failed to bind";

	listen(serverSock, 5);
}

/*
AcceptAndDispatch();
Main loop:
Blocks at accept(), until a new connection arrives.
When it happens, create a new thread to handle the new client.
*/
void Server::AcceptAndDispatch() {
	socklen_t cliSize = sizeof(sockaddr_in);

	while (true) {
		const auto c = new Client();
		auto t = new Thread();

		//Blocks here;
		c->sock = accept(serverSock, reinterpret_cast<struct sockaddr *>(&clientAddr), &cliSize);

		if (c->sock < 0) {
			cerr << "Error on accept";
		}
		else {
			t->Create(static_cast<void *>(HandleClient), c);
		}
	}
}


void *Server::HandleClient(void *args) {

	//Pointer to accept()'ed Client
	auto c = static_cast<Client *>(args);
	char buffer[256 - 25], message[256];

	//Add client in Static clients <vector> (Critical section!)
	Thread::LockMutex(static_cast<const char *>(c->name));

	//Before adding the new client, calculate its id. (Now we have the lock)
	c->SetId(clients.size());
	sprintf_s(buffer, "Client n.%d", c->id);
	c->SetName(buffer);
	cout << "Adding client with id: " << c->id << endl;
	clients.push_back(*c);

	Thread::UnlockMutex(static_cast<const char *>(c->name));

	while (true) {
		memset(buffer, 0, sizeof buffer);
		const int n = recv(c->sock, buffer, sizeof buffer, 0);

		//If client is disconnected
		if (n == 0) {
			cout << "Client " << c->name << " disconnected" << endl;
			_close(c->sock);

			//Remove client in Static clients <vector> (Critical section!)
			Thread::LockMutex(static_cast<const char *>(c->name));

			const int index = FindClientIndex(c);
			cout << "Erasing user in position " << index << " whose name id is: "
				<< clients[index].id << endl;

			clients.erase(clients.begin() + index);

			Thread::UnlockMutex(static_cast<const char *>(c->name));

			break;
		}
		if (n < 0) {
			cerr << "Error while receiving message from client: " << c->name << endl;
		}
		else {
			//Message received. Send to all clients.
			snprintf(message, sizeof message, "<%s>: %s", c->name, buffer);
			cout << "Will send to all: " << message << endl;
			SendToAll(message);
		}
	}

	//End thread
	return nullptr;
}

void Server::SendToAll(char *message) {
	//Acquire the lock
	Thread::LockMutex("'SendToAll()'");

	for (auto & client : clients) {
		const int n = send(client.sock, message, strlen(message), 0);
		cout << n << " bytes sent." << endl;
	}

	//Release the lock  
	Thread::UnlockMutex("'SendToAll()'");
}

void Server::ListClients() {
	for (auto & client : clients) {
		cout << client.name << endl;
	}
}

/*
Should be called when vector<Client> clients is locked!
*/
int Server::FindClientIndex(Client *c) {
	for (size_t i = 0; i<clients.size(); i++) {
		if (clients[i].id == c->id) return static_cast<int>(i);
	}
	cerr << "Client id not found." << endl;
	return -1;
}