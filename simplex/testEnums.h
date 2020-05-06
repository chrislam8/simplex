#pragma once

#include <utility>

enum testList {
	SIMPL_TEST = 0,
	NEG_SIZE_TEST,
	NEGROW_TEST,
	UNBOUNDED_TEST,
	INFEASIBLE_TEST,

	NUMBER_OF_TESTS
};

enum testErrorCodes {
	TEST_SUCCESS,
	TEST_INCORRECT_VALUE,
	TEST_INCORRECT_OPERATION,
	TEST_UNEXPECTED_VALUE,
	TEST_INCORRECT_OPTIMAL_VALUE

};

#define testResultCodes std::pair<testList, testErrorCodes>