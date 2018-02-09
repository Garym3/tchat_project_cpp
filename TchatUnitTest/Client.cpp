#include "Client.h"


Client::Client() = default;

/// <summary>
/// Sets a custom client pseudo
/// </summary>
/// <param pseudo="pseudo">Custom client pseudo</param>
bool Client::set_pseudo(const vector<Client> serverClients, const string& pseudo)
{
	for (const Client& client : serverClients)
	{
		if (pseudo == client.pseudo) return false;
	}

	this->pseudo = pseudo;

	return true;
}

/// <summary>
/// Sets a random client pseudo
/// </summary>
/// <param name="serverClients">Vector of all server' Clients</param>
/// <param name="number">Number to be incremented to generate random and unique pseudos</param>
void Client::set_pseudo(const vector<Client> serverClients, int number)
{
	string pseudo(generate_random_pseudo(number));

	for (const Client& client : serverClients)
	{
		if (pseudo == client.pseudo) {
			pseudo = set_pseudo_rec(serverClients, ++number);
		}
	}

	this->pseudo = pseudo;
}

/// <summary>
/// Recursive function for "set_pseudo()"
/// </summary>
/// <param name="serverClients">Vector of all server' Clients</param>
/// <param name="number">Number to be incremented to generate random and unique pseudos</param>
/// <returns>A unique pseudo</returns>
string Client::set_pseudo_rec(vector<Client> serverClients, int number)
{
	string pseudo(generate_random_pseudo(number));

	for (const Client& client : serverClients)
	{
		if (pseudo == client.pseudo) {
			pseudo = set_pseudo_rec(serverClients, ++number);
		}
	}

	return pseudo;
}

/// <summary>
/// Sets a client id to a Client
/// </summary>
/// <param name="serverClients">Vector of all server' Clients</param>
/// <param name="id">Number to be incremented to generate random and unique ids</param>
void Client::set_id(const vector<Client> serverClients, int id)
{
	for (const Client& client : serverClients)
	{
		if (id == client.id) {
			id = set_id_rec(serverClients, ++id);
		}
	}

	this->id = id;
}

/// <summary>
/// Recursive function for "set_id()"
/// </summary>
/// <param name="serverClients">Vector of all server' Clients</param>
/// <param name="id">Number to be incremented to generate random and unique ods</param>
/// <returns>A unique id</returns>
int Client::set_id_rec(vector<Client> serverClients, int id)
{
	for (const Client& client : serverClients)
	{
		if (id == client.id) {
			id = set_id_rec(serverClients, ++id);
		}
	}

	return id;
}

/// <summary>
/// Generates a predefined pseudo
/// </summary>
/// <param name="number">Number to be incremented</param>
/// <returns>A new pseudo</returns>
string Client::generate_random_pseudo(const int number)
{
	const string randomPseudo("random" + to_string(number));
	return randomPseudo;
}
