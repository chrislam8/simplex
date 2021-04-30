#pragma once

#include "testEnums.h"
#include "simpl.h"

class allTests {
public:
	testResultCodes runAllTests();

private:
	testErrorCodes simplTest();
	testErrorCodes negsizeTest();
	testErrorCodes negrowTest();
	testErrorCodes unboundedTest();
	testErrorCodes infeasibleTest();
	testErrorCodes exportTest();
	testErrorCodes importTest();
	testErrorCodes checkSimplex(simpl* test);
	testErrorCodes negativeTranspose();
	int NUMVAR = 2;
	int NUMCONSTR = 3;
	int NUMCOL = NUMVAR + 1;
	int NUMROW = NUMCONSTR + 1;
};