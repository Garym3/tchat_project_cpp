#include "CppUnitTest.h"
#include "../ESGI_Tchat_Project/Client.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TchatUnitTest
{		
	TEST_CLASS(ClientTest)
	{
	public:
		
		TEST_METHOD(NameShouldHaveLessOrSameLengthThanMaxNameLength)
		{
			Client client;
			const auto name = _strdup("LessThan50Characters");
			client.SetName(name);

			Assert::IsTrue(sizeof client.name <= MAX_NAME_LENGTH);
		}

		TEST_METHOD(NameShouldNotBeNullOrEmpty)
		{
			Client client;
			const auto name = _strdup(nullptr);
			client.SetName(name);

			Assert::IsFalse(client.name != nullptr);
		}

		TEST_METHOD(ShouldSetName)
		{
			Client client;
			const auto name = _strdup("name");
			client.SetName(name);

			Assert::IsTrue(client.name == name);
		}

		TEST_METHOD(ShouldSetId)
		{
			Client client;
			const int id = 5;
			client.SetId(id);

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