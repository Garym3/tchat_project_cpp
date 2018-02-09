#include "CppUnitTest.h"
#include "../ESGI_Tchat_Project/Client.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TchatUnitTest
{		
	TEST_CLASS(ClientTest)
	{
	public:
		vector<Client> serverClients;
		
		TEST_METHOD(NameShouldHaveLessOrSameLengthThanMaxNameLength)
		{
			Client client;
			const string name = "LessThan50Characters";
			client.set_pseudo(serverClients, name);

			Assert::IsTrue(client.pseudo.size() <= MAX_PSEUDO_LENGTH);
		}

		TEST_METHOD(NameShouldNotBeNullOrEmpty)
		{
			Client client;
			const string name = nullptr;
			client.set_pseudo(serverClients, name);

			Assert::IsFalse(client.pseudo.length() > 0 || client.pseudo != "\0");
		}

		TEST_METHOD(ShouldSetName)
		{
			Client client;
			const string name = "pseudo";
			client.set_pseudo(serverClients, name);

			Assert::IsTrue(client.pseudo == name);
		}

		TEST_METHOD(ShouldSetId)
		{
			Client client;
			const int id = 5;
			client.set_id(serverClients, id);

			Assert::IsTrue(client.id == id);
		}

		TEST_METHOD(SocketShouldBePositive)
		{
			Client client;
			const int socket = 5;
			client.socket = socket;

			Assert::IsTrue(client.socket >= 0);
		}

	};
}