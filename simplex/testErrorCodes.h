#pragma once

#include <utility>

enum testErrorCodes {
	TEST_SUCCESS,
	TEST_INCORRECT_VALUE,
	TEST_INCORRECT_OPERATION,
	TEST_UNEXPECTED_VALUE

};

#define testResultCodes std::pair<testList, testErrorCodes>