#include <iostream>
#include <string>

#include "test.h"

void runTests();

int main() 
{
	bool continuing = true;
	std::string input;
	while (continuing)
	{
		std::cout << "What is your command?" << std::endl;
		std::cout << "Command for exiting the program is 'exit'." << std::endl;
		//std::cout << "Command for help on other commands is 'help'." << std::endl;
		std::getline(std::cin, input);
		if (input == "exit")
		{
			continuing = false;
		}
		if (input == "test")
		{
			runTests();
		}
		std::cout << std::endl << std::endl;
	}
	return 0;
}

void runTests()
{
	allTests testSeq;
	testResultCodes testResult = testSeq.runAllTests();
	if (testResult.second != TEST_SUCCESS)
	{
		std::cout << "Test number #" << testResult.first << " failed with code " << testResult.second;
	}
	else
	{
		std::cout << "All tests succeeded";
	}
}