#ifndef _client_h_
#define _client_h_

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define MAX_NAME_LENGHT 20

using namespace std;

class Client {
public:
	char *name;
	int id{};
	int socket{};

	Client();
	void SetName(const char *name) const;
	void SetId(int id);
};

#endif