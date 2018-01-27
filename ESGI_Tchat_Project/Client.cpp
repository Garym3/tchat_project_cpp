#include "Client.h"


Client::Client() = default;

/// <summary>
/// Set a custom client pseudo
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
/// Set a random client pseudo
/// </summary>
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
/// Set client id
/// </summary>
/// <param pseudo="id"></param>
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

string Client::generate_random_pseudo(const int number)
{
	/*const auto randomChars = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		const size_t max_index = sizeof charset - 1;

		return charset[rand() % max_index];
	};

	string str(length, 0);
	std::generate_n(str.begin(), length, randomChars);

	return str;*/

	/*srand(time(nullptr));

	return to_string(rand() % maxNumber);*/

	/*mt19937 rng(time(nullptr));
	const uniform_int_distribution<int> gen(0, maxNumber);

	return to_string(gen(rng));*/
	const string randomPseudo("random" + to_string(number));
	return randomPseudo;
}
