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
		case EXPORT_TEST:
			errorCode = exportTest();
			break;
		case IMPORT_TEST:
			errorCode = importTest();
			break;
		case NEGATIVE_IMPORT_TEST:
			errorCode = negativeTranspose();
			break;
		}
		result.first = static_cast<testList>(testNumber);
		result.second = errorCode;
		if (errorCode != TEST_SUCCESS && errorCode != TEST_NOT_IMPLEMENTED_FUNCTION)
		{
			return result;
		}
	}
	return result;
}

testErrorCodes allTests::simplTest() {
	testErrorCodes result = TEST_SUCCESS;

	simpl* test = new simpl(NUMVAR, NUMCONSTR,false);
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

	result = checkSimplex(test);

	//free all dynamic memory used
	delete test;
	test = NULL;
	
	return result;
}

testErrorCodes allTests::negsizeTest() {
	simpl* test = new simpl(-1, -1,false);
	delete test;
	test = NULL;
	return TEST_SUCCESS;
}

testErrorCodes allTests::negrowTest() {
	simpl* test = new simpl(NUMROW,NUMCOL,false);
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
	simpl* test = new simpl(2, 2,false);
	double* xRes = new double[2];
	double optVal = -1;
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
	simpl* test = new simpl(2, 2,false);
	double* xRes = new double[2];
	double optVal = -1;
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

testErrorCodes allTests::exportTest()
{
	simpl* test = new simpl(NUMVAR, NUMCONSTR, false);
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
	test->exportTableau("output.csv");

	delete test;
	test = NULL;

	return TEST_SUCCESS;
}

testErrorCodes allTests::importTest()
{
	testErrorCodes result = TEST_SUCCESS;
	simpl* test = new simpl(NUMVAR, NUMCONSTR, false);
	double* xValuePointer = new double[NUMVAR];
	double optimalValue = 0;
	if (!(test->importTableau("input.csv")))
	{
		return TEST_UNEXPECTED_VALUE;
	}
	result = checkSimplex(test);
	test->exportTableau("input_processed.csv");

	delete test;
	test = NULL;

	return result;
}

testErrorCodes allTests::checkSimplex(simpl* test)
{
	double* correctxValues = new double[NUMVAR];
	correctxValues[0] = 10.0;
	correctxValues[1] = 5.0;
	double correctOptimalValue = 2750.0;
	double optimalValue = -8.2;
	double* testxVal = new double[NUMVAR];
	int variableIter;
	testErrorCodes result = TEST_SUCCESS;

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

	delete[] testxVal;
	testxVal = NULL;
	delete[] correctxValues;
	correctxValues = NULL;

	return result;
}

testErrorCodes allTests::negativeTranspose()
{
	simpl* test = new simpl(NUMVAR, NUMCONSTR, true);
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
	test->negativeTranspose();
	test->exportTableau("negTransposePost.csv");
	delete test;
	test = NULL;
	return TEST_NOT_IMPLEMENTED_FUNCTION;
}