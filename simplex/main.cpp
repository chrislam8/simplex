#include <iostream>

#include "test.h"

int main() {
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
	return 0;
}