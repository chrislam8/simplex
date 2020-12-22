#include <iostream>
#include <string>

#include "test.h"

void runTests();
void helpText();

int main() 
{
	bool continuing = true;
	std::string input;
	while (continuing)
	{
		bool realCommand = false;
		std::cout << "What is your command?" << std::endl;
		std::cout << "The command 'help' prints out the available commands." << std::endl;
		std::getline(std::cin, input);
		if (input == "exit")
		{
			continuing = false;
			realCommand = true;
		}
		else
		{
			std::cout << std::endl;
		}
		if (input == "test")
		{
			runTests();
			realCommand = true;
		}
		if (input == "help")
		{
			helpText();
			realCommand = true;
		}
		if (!realCommand)
		{
			std::cout << input << " is not a real command. Try again.";
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

void helpText()
{
	std::cout << "List of commands:" << std::endl << std::endl;
	std::cout << "exit - exits the program" << std::endl;
	std::cout << "help - prints the list of available commands" << std::endl;
	std::cout << "test - runs all program tests";
}