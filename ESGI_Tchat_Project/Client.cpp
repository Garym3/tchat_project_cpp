#include "Client.h"

using namespace std;

Client::Client() {
	this->name = static_cast<char *>(malloc(MAX_NAME_LENGHT + 1));
}

void Client::SetName(const char *name) const
{
	//Copies at most MAX_NAME_LENGHT + 1 (including '\0')
	snprintf(this->name, MAX_NAME_LENGHT + 1, name);
}

void Client::SetId(const int id) {
	this->id = id;
}