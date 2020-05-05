#pragma once

#include "testErrorCodes.h"

enum testList {
	SIMPL_TEST = 0,
	NEG_SIZE_TEST,
	NEGROW_TEST,
	UNBOUNDED_TEST,
	INFEASIBLE_TEST,

	NUMBER_OF_TESTS
};

class allTests {
public:
	testErrorCodes runAllTests();

private:
	testErrorCodes simplTest();
	testErrorCodes negsizeTest();
	testErrorCodes negrowTest();
	testErrorCodes unboundedTest();
	testErrorCodes infeasibleTest();

};