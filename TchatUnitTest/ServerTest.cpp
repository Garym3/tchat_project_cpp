#include "CppUnitTest.h"
#include "../ESGI_Tchat_Project/Client.h"
#include "../ESGI_Tchat_Project/Thread.h"
#include "../ESGI_Tchat_Project/Server.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TchatUnitTest
{
	TEST_CLASS(ServerTest)
	{
	public:

		TEST_METHOD(ShouldReadHistory)
		{
			ifstream reader("histo");
			Assert::IsTrue(reader.good());

			for (string line; getline(reader, line); )
			{
				line += global::newLine;
				Assert::IsNotNull(line.c_str());
			}

			reader.close();
		}

		TEST_METHOD(ShouldAppendToHistory)
		{
			ofstream writer("histo", ios_base::app);
			Assert::IsTrue(writer.good());

			const int oldSize = writer.tellp();

			writer << "New histo line" << endl;

			const int newSize = writer.tellp();

			Assert::IsTrue(oldSize <= newSize);

			writer.close();
		}

	};
}