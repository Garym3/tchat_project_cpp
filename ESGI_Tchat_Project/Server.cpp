#include "Server.h"

using namespace std;
using namespace global;

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
void Server::serverDeployment() {
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
			thread->Create(reinterpret_cast<void *>(handleClient), client);
		}
	}
}

/// <summary>
/// Handles client data and messages
/// </summary>
/// <param name="args">Callback arguments (Client)</param>
/// <returns>Null pointer</returns>
void *Server::handleClient(void *args) {
	//Pointer to the Client
	auto client = static_cast<Client*>(args);
	char message[300];
	bool isFirstMessage = true;

	//Add client in the vector of Clients
	Thread::LockMutex(to_string(clients.size()));

	//Before adding the new client, calculate its id while having the lock
	client->SetId(clients.size());
	sprintf_s(message, "Client %d", client->id);
	client->SetName(message);
	printf("Adding client with id: %d.%s", client->id, newLine.c_str());
	clients.push_back(*client);

	Thread::UnlockMutex(to_string(client->id));

	while (true) {
		//Resets the message
		memset(message, 0, sizeof message);

		const int nbBytes = recv(client->socket, message, sizeof message, 0);

		if(nbBytes > 0)
		{
			// Ignore return key chars when a client submits a message
			if (message[0] == 10 || message[0] == 13) continue;

			if(isFirstMessage) //Skip the very first weird string when connecting
			{
				string welcomeMessage("Welcome to the tchat :)" + newLine);
				welcomeMessage.append(R"(Type "\histo" to retrieve the 100 last messages.)" + newLine);
				sendTo(welcomeMessage, client->socket);

				isFirstMessage = false;
				continue;
			}
 
			if (message[0] == '\\')
			{
				const string command(message);
				if(command == R"(\histo)")
				{
					sendTo(newLine, client->socket);
					readHistoryAndSend("histo", client->socket);
					sendTo(newLine, client->socket);
				}
				continue;
			}

			appendToHistory("histo", message);
			printf("%sSending to all: %s%s", newLine.c_str(), message, newLine.c_str());

			// Send the message to all clients except the sender
			sendToAll(message, client->id);

			continue;
		}
		if (nbBytes == 0) { // The client is disconnected
			shutdownClient(client->socket);

			printf("Client %d disconnected.%s", client->id, newLine.c_str());

			//Lock Mutex in order to process following instructions
			Thread::LockMutex(to_string(client->id));

			const int index = findClientId(client);
			printf("Erasing user in position %d whose id is: %d.%s", index, clients[index].id, newLine.c_str());

			string disconnectionMessage(to_string(clients[index].id));
			disconnectionMessage.append(" has been disconnected from the server." + newLine);
			
			clients.erase(clients.begin() + index);			

			Thread::UnlockMutex(to_string(client->id));

			sendToAll(disconnectionMessage);

			break;
		}
		if (nbBytes < 0) { // Unknown error
			cerr << "Error while receiving message from client: " << client->name << endl;

			break;
		}
	}

	return nullptr;
}

int Server::sendTo(const string& message, const int clientSocket)
{
	return send(clientSocket, message.c_str(), message.length(), 0);
}

/// <summary>
/// WIP
/// </summary>
/// <param name="client"></param>
/// <param name="message"></param>
void Server::receive(const Client* client, const string& message)
{
	char* msg = _strdup(message.c_str());
	const int bytes = recv(client->socket, msg, sizeof msg, 0);

	if (bytes > 0)
	{
		
	}
	if (bytes == 0) { //If a client is disconnected
		
	}
	if (bytes < 0) { //Unknown error
		cerr << "Error while receiving message from client: " << client->name << endl;
	}
}

/// <summary>
/// Send the message to other clients
/// </summary>
/// <param name="message">Message to send</param>
void Server::sendToAll(const string& message, const int senderClientId) {
	Thread::LockMutex("'sendToAll()'");

	for (auto & client : clients) {
		if (client.id == senderClientId) continue;

		const string msg(message + newLine);
		const int nbBytes = sendTo(msg, client.socket);
		printf("%d bytes sents.%s", nbBytes, newLine.c_str());
	}

	//Release the lock  
	Thread::UnlockMutex("'sendToAll()'");
}

void Server::shutdownClient(const int clientSocket)
{
#ifdef _WIN32
	if (shutdown(clientSocket, SD_SEND) == SOCKET_ERROR) {
		printf("Shutdown failed: %d.%s", WSAGetLastError(), newLine.c_str());
		closesocket(clientSocket);
		WSACleanup();
	}
#else
	if (_close(client->socket) == -1)
	{
		printf("ERROR : Unable to close %d socket.%s", client->id, newLine.c_str());
	}
#endif
}

/// <summary>
/// Get client id
/// Should be called when vector<Client> clients is locked.
/// </summary>
/// <param name="client">Client</param>
/// <returns>Client id or -1 for error handling</returns>
int Server::findClientId(Client *client) {
	for (size_t i = 0; i<clients.size(); i++) {
		if (clients[i].id == client->id) return static_cast<int>(i);
	}
	cerr << "Client id not found." << endl;
	return -1;
}

/// <summary>
/// Reads history file and send line by line to client
/// </summary>
/// <param name="filePath">Path to the history file</param>
/// <param name="client">Current Client</param>
void Server::readHistoryAndSend(const string & filePath, const int clientSocket)
{
	ifstream reader(filePath);

	for (string line; getline(reader, line); )
	{
		line += newLine;
		sendTo(line, clientSocket);
	}

	reader.close();
}

/// <summary>
/// Write in history file line by line
/// </summary>
/// <param name="filePath">Path to the history file</param>
/// <param name="message">Message, from the Client, to write in the history file</param>
void Server::appendToHistory(const string& filePath, const string& message)
{
	ofstream writer(filePath, ios_base::app);

	writer << message << endl;

	writer.close();
}
