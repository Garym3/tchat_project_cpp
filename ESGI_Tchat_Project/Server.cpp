#include "Server.h"
#include <conio.h>

using namespace std;

vector<Client> Server::clients;

Server::Server() {
	//Initializes a static Mutex from Thread
	Thread::InitMutex();

	//For setsock opt (REUSEADDR)
	char yes = 1;

	//Inits serverSocket and starts listening
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	//Avoids binding error if the socket was not closed last time
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(sockaddr_in)) < 0)
		cerr << "Failed to bind";

	listen(serverSocket, 5);
}

/// <summary>
/// Main loop:
/// Blocks at accept() until a new connection arrives.
/// When it happens, creates a new thread to handle the new client.
/// </summary>
void Server::AcceptAndDispatch() {
	socklen_t clientAddrSize = sizeof(sockaddr_in);

	while (true) {
		const auto client = new Client();
		auto thread = new Thread();

		//Blocks here
		client->socket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddrSize);

		if (client->socket < 0) {
			cerr << "Error on accept";
		}
		else {
			thread->Create(static_cast<void *>(HandleClient), client);
		}
	}
}

/// <summary>
/// Handles client data and messages
/// </summary>
/// <param name="args">Callback arguments</param>
/// <returns>Void* pointer</returns>
void *Server::HandleClient(void *args) {
	//Pointer to the Client
	auto client = static_cast<Client*>(args);
	char message[300];

	//Add client in the vector of Clients
	Thread::LockMutex(static_cast<const char *>(client->name));

	//Before adding the new client, calculate its id while having the lock
	client->SetId(clients.size());
	sprintf_s(message, "Client %d", client->id);
	client->SetName(message);
	cout << "Adding client with id: " << client->id << endl;
	clients.push_back(*client);

	Thread::UnlockMutex(static_cast<const char *>(client->name));

	while (true) {
		// Resets the message
		memset(message, 0, sizeof message);
		const int count = recv(client->socket, message, sizeof message, 0);
		
		if(count > 0)
		{
			//Message received. Send to all clients except the sender one
			//snprintf(message, sizeof message, "<%s>: %s", client->name, buffer);
			cout << "Will send to all: " << message << endl;
			SendToAll(message, client->id);
			continue;
		}
		//If a client is disconnected
		if (count == 0) {
			cout << "Client " << client->name << " disconnected" << endl;
			_close(client->socket);

			//Remove client in Static clients <vector> (Critical section!)
			Thread::LockMutex(static_cast<const char *>(client->name));

			const int index = FindClientId(client);
			cout << "Erasing user in position " << index << " whose name id is: "
				<< clients[index].id << endl;

			clients.erase(clients.begin() + index);

			Thread::UnlockMutex(static_cast<const char *>(client->name));

			break;
		}
		if (count < 0) {
			cerr << "Error while receiving message from client: " << client->name << endl;
		}
	}

	return nullptr;
}

/// <summary>
/// Send the message to other clients
/// </summary>
/// <param name="message">Message to send</param>
void Server::SendToAll(char *message, const int senderClientId) {
	Thread::LockMutex("'SendToAll()'");

	for (auto & client : clients) {
		// Only send to other clients
		if (client.id == senderClientId) continue;
		
		const int n = send(client.socket, message, strlen(message), 0);
		cout << n << " bytes sent." << endl;
	}

	//Release the lock  
	Thread::UnlockMutex("'SendToAll()'");
}

/// <summary>
/// Lists all connected clients
/// </summary>
void Server::ListClients() {
	for (auto & client : clients) {
		cout << client.name << endl;
	}
}

/// <summary>
/// Get client id
/// Should be called when vector<Client> clients is locked.
/// </summary>
/// <param name="client">Client</param>
/// <returns>Client id or -1 for error handling</returns>
int Server::FindClientId(Client *client) {
	for (size_t i = 0; i<clients.size(); i++) {
		if (clients[i].id == client->id) return static_cast<int>(i);
	}
	cerr << "Client id not found." << endl;
	return -1;
}