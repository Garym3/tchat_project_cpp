#pragma once

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

class Client {
public:
	string pseudo;
	int id{};
	int socket{};

	Client();
	bool set_pseudo(vector<Client> serverClients, const string& pseudo);
	void set_pseudo(vector<Client> serverClients, int number = 0);
	static string set_pseudo_rec(vector<Client> serverClients, int number);
	void set_id(vector<Client> serverClients, int id = 0);
	static int set_id_rec(vector<Client> serverClients, int id);
	static string generate_random_pseudo(int number);
};