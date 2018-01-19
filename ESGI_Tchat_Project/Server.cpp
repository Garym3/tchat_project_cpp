#include "Server.h"
#include "Global.h"

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
/// <returns>Void* pointer</returns>
void *Server::handleClient(void *args) {
	//Pointer to the Client
	auto client = static_cast<Client*>(args);
	char message[300];
	bool skipFirst = true;

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
		//Resets the message
		memset(message, 0, sizeof message);

		//Sends a random number of packets...
		const int count = recv(client->socket, message, sizeof message, 0);

		//... so a check must be done
		if(count > 0)
		{
			// Ignore return key chars when a client submits a message
			if (message[0] == 10 || message[0] == 13) continue;

			if(skipFirst) //Skip the very first weird string when connecting
			{
				string welcomeMessage("Welcome to the tchat." + newLine);
				send(client->socket, welcomeMessage.c_str(), welcomeMessage.length(), 0);
				bool loopMenu = true;

				while(loopMenu)
				{
					readFromHistoryAndSend("histo", client);
					loopMenu = false;
				}				

				skipFirst = false;
				continue;
			}
 
			cout << endl << "Sending to all: " << message << endl << endl;

			writeInHistory("histo", message);

			// Send the message to all clients except the sender
			sendToAll(message, client->id);
			continue;
		}
		if (count == 0) { //If a client is disconnected
			cout << "Client " << client->name << " disconnected" << endl;
			//if(_close(client->socket) == -1)
			//{
			//	cout << "ERROR : Unable to close " << client->name << " socket." << endl;
			//}

			//Lock Mutex in order to process following instructions
			Thread::LockMutex(static_cast<const char *>(client->name));

			const int index = findClientId(client);
			cout << "Erasing user in position " << index << " whose name id is: "
				<< clients[index].id << endl;

			//Remove client from the vector of Clients
			clients.erase(clients.begin() + index);

			Thread::UnlockMutex(static_cast<const char *>(client->name));

			break;
		}
		if (count < 0) { //Unknown error
			cerr << "Error while receiving message from client: " << client->name << endl;
		}
	}

	return nullptr;
}

int Server::sendTo(const Client* client, const string& message)
{
	return send(client->socket, message.c_str(), message.length(), 0);
}

void Server::receive(const Client* client, const string& message)
{
	char* msg = _strdup(message.c_str());
	const int bytes =  recv(client->socket, msg, sizeof msg, 0);

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
void Server::sendToAll(char *message, const int senderClientId) {
	Thread::LockMutex("'sendToAll()'");

	for (auto & client : clients) {
		// Only send to other clients
		if (client.id == senderClientId) continue;		

		string mess(message + newLine);
		const int n = send(client.socket, mess.c_str(), mess.length(), 0);
		cout << n << " bytes sent." << endl;
	}

	//Release the lock  
	Thread::UnlockMutex("'sendToAll()'");
}

/// <summary>
/// Lists all connected clients
/// UNUSED FOR NOW
/// </summary>
void Server::listClients() {
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
void Server::readFromHistoryAndSend(const string & filePath, const Client* client)
{
	ifstream reader(filePath);

	for (string line; getline(reader, line); )
	{
		line += newLine;
		sendTo(client, line);
	}

	reader.close();
}

/// <summary>
/// Write in history file line by line
/// </summary>
/// <param name="filePath">Path to the history file</param>
/// <param name="message">Message, from the Client, to write in the history file</param>
void Server::writeInHistory(const string& filePath, const string& message)
{
	ofstream writer(filePath, ios_base::app);

	writer << message << endl;

	writer.close();
}
