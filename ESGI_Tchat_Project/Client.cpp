#include "Client.h"

using namespace std;

Client::Client() {
	this->name = static_cast<char *>(malloc(MAX_NAME_LENGHT + 1));
}

/// <summary>
/// Set client name and copies at most MAX_NAME_LENGHT + 1 (including '\0')
/// </summary>
/// <param name="name"></param>
void Client::SetName(const char *name) const
{
	snprintf(this->name, MAX_NAME_LENGHT + 1, name);
}

/// <summary>
/// Set client id
/// </summary>
/// <param name="id"></param>
void Client::SetId(const int id) {
	this->id = id;
}