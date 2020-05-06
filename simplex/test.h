#pragma once

#include "testEnums.h"

class allTests {
public:
	testResultCodes runAllTests();

private:
	testErrorCodes simplTest();
	testErrorCodes negsizeTest();
	testErrorCodes negrowTest();
	testErrorCodes unboundedTest();
	testErrorCodes infeasibleTest();

};