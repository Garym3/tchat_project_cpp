#include "Server.h"

vector<Client> Server::clients;

Server::Server() {
	//Initializes a static Mutex from Thread
	Thread::init_mutex();

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
void Server::server_deployment() {
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
			thread->create(reinterpret_cast<void *>(handle_client), client);
		}
	}
}

/// <summary>
/// Handles client data and messages
/// </summary>
/// <param pseudo="args">Callback arguments (Client)</param>
/// <returns>Null pointer</returns>
void *Server::handle_client(void *args) {
	//Pointer to the Client
	auto client = static_cast<Client*>(args);
	char buffer[500];
	bool isFirstMessage = true;

	//Add client in the vector of Clients
	Thread::lock_mutex("N/A");

	//Before adding the new client, calculate its id while having the lock
	client->set_id(clients, clients.size());
	client->set_pseudo(clients);
	printf("Adding client with pseudo: %s.%s", client->pseudo.c_str(), newLine.c_str());
	clients.push_back(*client);

	Thread::unlock_mutex(client->pseudo);

	while (true) {
		memset(buffer, 0, sizeof buffer);

		const int nbBytes = recv(client->socket, buffer, sizeof buffer, 0);

		string message(buffer);

		if(nbBytes > 0)
		{
			isFirstMessage = handle_data(client, &message[0u], isFirstMessage);

			continue;
		}
		if (nbBytes == 0) { // The client is disconnected
			shutdown_client(client->socket);

			printf("Client %s disconnected.%s", client->pseudo.c_str(), newLine.c_str());

			//Lock Mutex in order to process following instructions
			Thread::lock_mutex(client->pseudo);

			const int index = find_client_id(client);
			printf("Erasing user in position %d whose pseudo is: %s.%s", index, clients[index].pseudo.c_str(), newLine.c_str());

			string disconnectionMessage(clients[index].pseudo);
			disconnectionMessage.append(" has been disconnected from the server." + newLine);
			
			clients.erase(clients.begin() + index);			

			Thread::unlock_mutex(client->pseudo);

			send_to_all(disconnectionMessage, -1);

			break;
		}
		if (nbBytes < 0) { // Unknown error
			cerr << "Error while receiving message from client: " << client->pseudo << endl;

			break;
		}
	}

	return nullptr;
}

vector<Client> Server::get_server_clients()
{
	return clients;
}

int Server::send_to(const string& message, const int clientSocket)
{
	return send(clientSocket, message.c_str(), message.length(), 0);
}

/// <summary>
/// WIP
/// </summary>
/// <param pseudo="client"></param>
/// <param pseudo="message"></param>
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
		cerr << "Error while receiving message from client: " << client->pseudo << endl;
	}
}

/// <summary>
/// Send the message to other clients
/// </summary>
/// <param pseudo="message">Message to send</param>
void Server::send_to_all(const string& message, const int senderClientId) {
	Thread::lock_mutex("'send_to_all()'");

	for (auto & client : clients) {
		if (client.id == senderClientId) continue;

		const string msg(message + newLine);
		const int nbBytes = send_to(msg, client.socket);
		printf("%d bytes sents.%s", nbBytes, newLine.c_str());
	}

	//Release the lock  
	Thread::unlock_mutex("'send_to_all()'");
}

void Server::shutdown_client(const int clientSocket)
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

bool Server::handle_data(Client* client, char* message, const bool isFirstMessage)
{
	// Ignore return key chars when a client submits a message
	if (message[0] == 10 || message[0] == 13) return isFirstMessage;

	if (isFirstMessage) //Skip the very first weird string when connecting
	{
		string welcomeMessage("Welcome to the tchat :)" + newLine);
		welcomeMessage.append(R"(Type "\help" to show available commands.)" + newLine);
		send_to(welcomeMessage, client->socket);

		return false;
	}

	if (message[0] == '\\')
	{
		const string command(message);

		if (command == R"(\help)")
		{
			string commands("Available commands:" + newLine);
			commands.append(R"(--- Show available commands "\help")" + newLine);
			commands.append(R"(--- Show the 50 last messages "\histo")" + newLine);
			commands.append(R"(--- Set a new pseudo "\pseudo YourNewPseudo")" + newLine);

			send_to(commands, client->socket);
		}
		else if (command == R"(\histo)")
		{
			send_to("---------- HISTORY ----------" + newLine, client->socket);
			History::read_history_and_send("histo", client->socket, 50);
			send_to("---------- END HISTORY ----------" + newLine, client->socket);
		}
		else if (command.find(R"(\pseudo)") != std::string::npos)
		{
			const string newPseudo(command.substr(sizeof R"(\pseudo)", command.size() - 1));
			const string oldPseudo(client->pseudo);
			Thread::lock_mutex(client->pseudo);
			
			client->set_pseudo(clients, newPseudo);

			Thread::unlock_mutex(client->pseudo);
			send_to_all(oldPseudo + " has become " + newPseudo, client->id);
		}

		return isFirstMessage;
	}

	string msgToSend(client->pseudo + ": ");
	msgToSend.append(message);

	History::append_to_history("histo", msgToSend);
	printf("%sSending to all: %s%s", newLine.c_str(), msgToSend.c_str(), newLine.c_str());

	// Send the message to all clients except the sender
	send_to_all(msgToSend, client->id);

	return isFirstMessage;
}

/// <summary>
/// Get client id
/// Should be called when vector<Client> clients is locked.
/// </summary>
/// <param pseudo="client">Client</param>
/// <returns>Client id or -1 for error handling</returns>
int Server::find_client_id(Client *client)
{
	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i].id != client->id) continue;

		return static_cast<int>(i);
	}

	cerr << "Client id not found." << endl;

	return -1;
}
