#include <iostream>
#include <string>

#include "test.h"

void runTests();
void helpText();
void about();

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
		if (input == "about")
		{
			about();
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
	std::cout << "about - displays some information about this program" << std::endl;
	std::cout << "exit - exits the program" << std::endl;
	std::cout << "help - prints the list of available commands" << std::endl;
	std::cout << "test - runs all program tests";
}

void about()
{
	std::cout << "Program Name: Linear Programming Calculator" << std::endl;
	std::cout << "Author: Christopher Lam" << std::endl;
	std::cout << "Version: 1.0.0.0" << std::endl;
	std::cout << "Last updated: December 21, 2020" << std::endl;
}