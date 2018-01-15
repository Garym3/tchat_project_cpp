#include "Client.h"

using namespace std;

Client::Client() {
	this->name = static_cast<char *>(malloc(MAX_NAME_LENGTH + 1));
}

/// <summary>
/// Set client name and copies at most MAX_NAME_LENGTH + 1 (including '\0')
/// </summary>
/// <param name="name"></param>
void Client::SetName(char *name)
{
	this->name = name;
}

/// <summary>
/// Set client id
/// </summary>
/// <param name="id"></param>
void Client::SetId(const int id) {
	this->id = id;
}