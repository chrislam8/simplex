#pragma once

#include "testEnums.h"

class allTests {
public:
	testResultCodes runAllTests();
	void exportTest();

private:
	testErrorCodes simplTest();
	testErrorCodes negsizeTest();
	testErrorCodes negrowTest();
	testErrorCodes unboundedTest();
	testErrorCodes infeasibleTest();
	int NUMVAR = 2;
	int NUMCONSTR = 3;
	int NUMCOL = NUMVAR + 1;
	int NUMROW = NUMCONSTR + 1;
};