#include <iostream>
#include <string>
#include "simpl.h"
#include "test.h"

testResultCodes allTests::runAllTests () {
	testErrorCodes errorCode = TEST_SUCCESS;
	testResultCodes result;
	for (int testNumber = 0; testNumber < NUMBER_OF_TESTS; testNumber++)
	{
		switch (testNumber)
		{
		case SIMPL_TEST:
			errorCode = simplTest();
			break;
		case NEG_SIZE_TEST:
			errorCode = negsizeTest();
			break;
		case NEGROW_TEST:
			errorCode = negrowTest();
			break;
		case UNBOUNDED_TEST:
			errorCode = unboundedTest();
			break;
		case INFEASIBLE_TEST:
			errorCode = infeasibleTest();
			break;
		}
		result.first = static_cast<testList>(testNumber);
		result.second = errorCode;
		if (errorCode != TEST_SUCCESS)
		{
			return result;
		}
	}
	return result;
}

testErrorCodes allTests::simplTest() {
	float* correctxValues = new float[NUMVAR];
	correctxValues[0] = 10.0;
	correctxValues[1] = 5.0;
	float correctOptimalValue = 2750.0;
	testErrorCodes result = TEST_SUCCESS;

	simpl* test = new simpl(NUMVAR, NUMCONSTR);
	int variableIter;
	float optimalValue = -8.2;
	float* testxVal = new float[NUMVAR];
	test->changeValue(1.0, 1, 1);
	test->changeValue(2.0, 1, 2);
	test->changeValue(20.0, 1, 3);
	test->changeValue(2.0, 2, 1);
	test->changeValue(2.0, 2, 2);
	test->changeValue(30.0, 2, 3);
	test->changeValue(2.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(25.0, 3, 3);
	test->changeValue(200.0, 4, 1);
	test->changeValue(150.0, 4, 2);
	test->changeValue(0.0, 4, 3);

	//simplex testing
	if (test->simplex(testxVal, &optimalValue) != SIMPLEX_SUCCESS)
	{
		result = TEST_SIMPLEX_ERROR;
	}

	for (variableIter = 0; variableIter < NUMVAR; variableIter++) {
		if (testxVal[variableIter] != correctxValues[variableIter])
		{
			result = TEST_INCORRECT_VALUE;
		}
	}
	
	if (optimalValue != correctOptimalValue)
	{
		result = TEST_INCORRECT_OPTIMAL_VALUE;
	}

	//free all dynamic memory used
	delete test;
	test = NULL;
	delete [] testxVal;
	testxVal = NULL;
	delete [] correctxValues;
	correctxValues = NULL;

	return result;
}

testErrorCodes allTests::negsizeTest() {
	simpl* test = new simpl(-1, -1);
	delete test;
	test = NULL;
	return TEST_SUCCESS;
}

testErrorCodes allTests::negrowTest() {
	simpl* test = new simpl(NUMROW,NUMCOL);
	testErrorCodes result = TEST_SUCCESS;
	if (test->changeValue(10.0, -1, 2)) {
		result = TEST_INCORRECT_OPERATION;
	}
	if (test->changeValue(10.0, 2, -1)) {
		result = TEST_INCORRECT_OPERATION;
	}
	if (!(test->changeValue(-10.0, 2, 2))) {
		result = TEST_BLOCKED_CORRECT_OPERATION;
	}
	delete test;
	test = NULL;
	return result;
}

testErrorCodes allTests::unboundedTest() {
	simpl* test = new simpl(2, 2);
	float* xRes = new float[2];
	float optVal = -1;
	testErrorCodes result = TEST_SUCCESS;
	test->changeValue(1.0, 1, 1);
	test->changeValue(-2.0, 1, 2);
	test->changeValue(-1.0, 1, 3);
	test->changeValue(-1.0, 2, 1);
	test->changeValue(1.0, 2, 2);
	test->changeValue(-1.0, 2, 3);
	test->changeValue(1.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(0.0, 3, 3);
	if (test->simplex(xRes, &optVal) != UNBOUNDED_PROBLEM) {
		result = TEST_INCORRECT_OPERATION;
	}
	delete test;
	test = NULL;
	return result;
}


testErrorCodes allTests::infeasibleTest() {
	simpl* test = new simpl(2, 2);
	float* xRes = new float[2];
	float optVal = -1;
	testErrorCodes result = TEST_SUCCESS;
	test->changeValue(-1.0, 1, 1);
	test->changeValue(2.0, 1, 2);
	test->changeValue(-5.0, 1, 3);
	test->changeValue(1.0, 2, 1);
	test->changeValue(-1.0, 2, 2);
	test->changeValue(-5.0, 2, 3);
	test->changeValue(1.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(0.0, 3, 3);
	if (test->simplex(xRes, &optVal) != NO_FEASIBLE_SOLUTION) {
		result = TEST_INCORRECT_OPERATION;
	}
	delete test;
	test = NULL;
	return result;
}

void allTests::exportTest()
{
	simpl* test = new simpl(NUMVAR, NUMCONSTR);
	float optimalValue = -8.2;
	float* testxVal = new float[NUMVAR];
	test->changeValue(1.0, 1, 1);
	test->changeValue(2.0, 1, 2);
	test->changeValue(20.0, 1, 3);
	test->changeValue(2.0, 2, 1);
	test->changeValue(2.0, 2, 2);
	test->changeValue(30.0, 2, 3);
	test->changeValue(2.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(25.0, 3, 3);
	test->changeValue(200.0, 4, 1);
	test->changeValue(150.0, 4, 2);
	test->changeValue(0.0, 4, 3);
	test->exportTableau();
}