#include <iostream>
#include <string>
#include "simpl.h"
#include "test.h"


#define NUMVAR 2
#define NUMCONSTR 3
#define NUMCOL 3
#define NUMROW 4

using namespace std;

testErrorCodes allTests::runAllTests () {
	testErrorCodes result = TEST_SUCCESS;
	for (int testNumber = 0; testNumber < NUMBER_OF_TESTS; testNumber++)
	{
		switch (testNumber)
		{
		case SIMPL_TEST:
			result = simplTest();
			break;
		case NEG_SIZE_TEST:
			result = negsizeTest();
			break;
		case NEGROW_TEST:
			result = negrowTest();
			break;
		case UNBOUNDED_TEST:
			result = unboundedTest();
			break;
		case INFEASIBLE_TEST:
			result = infeasibleTest();
			break;
		}
	}
	return result;
}

testErrorCodes allTests::simplTest() {
	simpl* test = new simpl(NUMVAR, NUMCONSTR);
	bool isErr = false;
	int variableIter;
	float testNum = -8.2;
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
	test->simplex(testxVal, &testNum);
	for (variableIter = 0; variableIter < NUMVAR; variableIter++) {
		cout << "x #" << (variableIter + 1) << " is " << testxVal[variableIter] << endl;
	}
	cout << "The optimal value is " << testNum << endl;
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	//free all dynamic memory used
	delete test;
	test = NULL;

	cout << "All good\n";
	return TEST_SUCCESS;
}

testErrorCodes allTests::negsizeTest() {
	simpl* test = new simpl(-1, -1);
	delete test;
	test = NULL;
	cout << "Negative size test complete" << endl;
	return TEST_SUCCESS;
}

testErrorCodes allTests::negrowTest() {
	simpl* test = new simpl(NUMROW,NUMCOL);
	if (test->changeValue(10.0, -1, 2)) {
		cout << "changeValue with negative row value test failed" << endl;
	}
	else {
		cout << "changeValue with negative row value test passed" << endl;
	}
	if (test->changeValue(10.0, 2, -1)) {
		cout << "changeValue with negative column value test failed" << endl;
	}
	else {
		cout << "changeValue with negative column value test passed" << endl;
	}
	if (test->changeValue(-10.0, 2, 2)) {
		cout << "changeValue with negative value test passed" << endl;
	}
	else {
		cout << "changeValue with negative value test failed" << endl;
	}
	delete test;
	test = NULL;
	cout << "Negative input test (other than size) complete" << endl;
	return TEST_SUCCESS;
}

testErrorCodes allTests::unboundedTest() {
	simpl* test = new simpl(2, 2);
	float* xRes = new float[2];
	float optVal = -1;
	bool testSuccess = true;
	test->changeValue(1.0, 1, 1);
	test->changeValue(-2.0, 1, 2);
	test->changeValue(-1.0, 1, 3);
	test->changeValue(-1.0, 2, 1);
	test->changeValue(1.0, 2, 2);
	test->changeValue(-1.0, 2, 3);
	test->changeValue(1.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(0.0, 3, 3);
	if (test->simplex(xRes, &optVal) != 2) {
		testSuccess = false;
	}
	delete test;
	test = NULL;
	if (testSuccess) {
		cout << "Unbounded test successful" << endl;
	}
	else {
		cout << "Unbounded test failed" << endl;
	}
	cout << "Unbounded test complete" << endl;
	return TEST_SUCCESS;
}


testErrorCodes allTests::infeasibleTest() {
	simpl* test = new simpl(2, 2);
	float* xRes = new float[2];
	float optVal = -1;
	bool testSuccess = true;
	test->changeValue(-1.0, 1, 1);
	test->changeValue(2.0, 1, 2);
	test->changeValue(-5.0, 1, 3);
	test->changeValue(1.0, 2, 1);
	test->changeValue(-1.0, 2, 2);
	test->changeValue(-5.0, 2, 3);
	test->changeValue(1.0, 3, 1);
	test->changeValue(1.0, 3, 2);
	test->changeValue(0.0, 3, 3);
	if (test->simplex(xRes, &optVal) != 1) {
		testSuccess = false;
	}
	delete test;
	test = NULL;
	if (testSuccess) {
		cout << "Infeasible test successful" << endl;
	}
	else {
		cout << "Infeasible test failed" << endl;
	}
	cout << "Infeasible test complete" << endl;
	return TEST_SUCCESS;
}
