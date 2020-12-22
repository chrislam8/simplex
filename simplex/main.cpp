#include <iostream>
#include <string>

#include "test.h"

void runTests();

int main() 
{
	bool continuing = true;
	while (continuing)
	{
		std::cout << "What is your command?" << std::endl;
		std::string input;
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