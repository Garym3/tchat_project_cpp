#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

using namespace std;

class Client {
public:
	char* name;
	int id{};
	int socket{};

	Client();
	void SetName(char *name);
	void SetId(int id);
};