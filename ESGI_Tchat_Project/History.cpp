#include "History.h"

History::History() = default;


History::~History() = default;

/// <summary>
/// Reads history file and send line by line to client
/// </summary>
/// <param pseudo="filePath">Path to the history file</param>
/// <param pseudo="client">Current Client</param>
void History::read_history_and_send(const string& filePath, const int clientSocket, const int numberOfLines)
{
	ifstream reader(filePath);
	deque<string> deque;
	int nbFileLines = 0;

	reader.seekg(ios_base::beg);

	for (string line; getline(reader, line);)
	{
		++nbFileLines;
	}

	reader.clear();
	reader.seekg(ios_base::beg);

	for (int i = 1; i <= nbFileLines; i++)
	{
		string line;
		getline(reader, line);

		if (i > nbFileLines - numberOfLines)
		{
			line += newLine;
			istringstream iss(line);
			deque.push_back(iss.str());
		}
	}

	reader.close();

	int lineCount = 1;

	while (!deque.empty() || lineCount <= numberOfLines)
	{
		Server::send_to(deque.front(), clientSocket);
		deque.pop_front();
		lineCount++;
	}	
}

/// <summary>
/// Write in history file line by line
/// </summary>
/// <param pseudo="filePath">Path to the history file</param>
/// <param pseudo="message">Message, from the Client, to write in the history file</param>
void History::append_to_history(const string& filePath, const string& message)
{
	ofstream writer(filePath, ios_base::app);

	writer << message << endl;

	writer.close();
}
