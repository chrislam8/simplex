#include <iostream>

#include "test.h"

int main() {
	allTests testSeq;
	testResultCodes testResult = testSeq.runAllTests();
	if (testResult.second != TEST_SUCCESS)
	{
		std::cout << "Error: test failed";
	}
	else
	{
		std::cout << "All tests succeeded";
	}
	return 0;
}