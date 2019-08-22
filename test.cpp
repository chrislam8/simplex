#include <iostream>
#include <string>
#include "simpl.h"


#define NUMVAR 2
#define NUMCONSTR 3
#define NUMCOL 3
#define NUMROW 4

using namespace std;

void pivotTest();
void simplTest();
void negsizeTest();
void negrowTest();
void unboundedTest();
void infeasibleTest();
void minimizationTest();

int main () {
	pivotTest();
	simplTest();
	negsizeTest();
	negrowTest();
	unboundedTest();
	infeasibleTest();
	minimizationTest();
	return 0;
}

void pivotTest() {
	simpl* test = new simpl(NUMVAR, NUMCONSTR, false);
	bool isErr = false;
	//float** tableauCheck = new float*[NUMROW];
	int i, j;
	float testNum = -8.2;
	/* for (i = 0; i < NUMROW; i++) {
		tableauCheck[i] = new float[NUMCOL];
	} */
	//initialize values of tableau
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
	//looking at the values in the tableau
	test->prTableau();
	/*for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}*/
	//pivot tableau
	if (!(test->pivot(3, 1))) {
		cout << "Error: pivot returned false when it should return true" << endl;
		isErr = true;
	}
	test->prTableau();
	if (!(test->pivot(2, 2))) {
		cout << "Error: pivot returned false when it should return true" << endl;
		isErr = true;
	}
	//looking at the values in the tableau after pivoting
	test->prTableau();
	/*for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}*/
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	//free all dynamic memory used
	delete test;
	test = NULL;
	/*for (i = 0; i < NUMROW; i++) {
		delete tableauCheck[i];
		tableauCheck[i] = NULL;
	}
	delete tableauCheck;
	tableauCheck = NULL;*/
	cout << "All good\n";
}

void simplTest() {
	simpl* test = new simpl(NUMVAR, NUMCONSTR,false);
	bool isErr = false;
	//float** tableauCheck = new float*[NUMROW];
	int i, j;
	float testNum = -8.2;
	float* testxVal = new float[NUMVAR];
	/* for (i = 0; i < NUMROW; i++) {
		tableauCheck[i] = new float[NUMCOL];
	} */
	//initialize values of tableau
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
	//looking at the values in the tableau
	test->prTableau();
	/*for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}*/
	//simplex testing
	test->simplex(testxVal, &testNum, NULL, NULL);
	//looking at the values in the tableau after pivoting
	test->prTableau();
	/*for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}*/
	for (i = 0; i < NUMVAR; i++) {
		cout << "x #" << (i + 1) << " is " << testxVal[i] << endl;
	}
	cout << "The optimal value is " << testNum << endl;
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	//free all dynamic memory used
	delete test;
	test = NULL;
	/*for (i = 0; i < NUMROW; i++) {
		delete tableauCheck[i];
		tableauCheck[i] = NULL;
	}
	delete tableauCheck;
	tableauCheck = NULL;*/
	cout << "All good\n";
}

void negsizeTest() {
	simpl* test = new simpl(-1, -1, false);
	test->prTableau();
	delete test;
	test = NULL;
	cout << "Negative size test complete" << endl;
}

void negrowTest() {
	simpl* test = new simpl(NUMROW,NUMCOL,false);
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
}

void unboundedTest() {
	simpl* test = new simpl(2, 2, false);
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
	test->prTableau();
	if (test->simplex(xRes, &optVal,NULL,NULL) != 2) {
		testSuccess = false;
	}
	test->prTableau();
	delete test;
	test = NULL;
	if (testSuccess) {
		cout << "Unbounded test successful" << endl;
	}
	else {
		cout << "Unbounded test failed" << endl;
	}
	cout << "Unbounded test complete" << endl;
}


void infeasibleTest() {
	simpl* test = new simpl(2, 2,false);
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
	test->prTableau();
	if (test->simplex(xRes, &optVal, NULL, NULL) != 1) {
		testSuccess = false;
	}
	test->prTableau();
	delete test;
	test = NULL;
	if (testSuccess) {
		cout << "Infeasible test successful" << endl;
	}
	else {
		cout << "Infeasible test failed" << endl;
	}
	cout << "Infeasible test complete" << endl;
}

void minimizationTest() {
	simpl* test = new simpl(2, 2, true);
	float* xRes = new float[2];
	float optVal = -1;
	bool testSuccess = true;
	int testVal = 0;
	test->changeValue(20.0, 1, 1);
	test->changeValue(25.0, 1, 2);
	test->changeValue(300.0, 1, 3);
	test->changeValue(40.0, 2, 1);
	test->changeValue(20.0, 2, 2);
	test->changeValue(500.0, 2, 3);
	test->changeValue(1000.0, 3, 1);
	test->changeValue(800.0, 3, 2);
	test->changeValue(0.0, 3, 3);
	test->prTableau();
	testVal = test->simplex(xRes, &optVal, NULL, NULL);
	if (testVal != 0) {
		testSuccess = false;
	}
	test->prTableau();
	delete test;
	test = NULL;
	if (testSuccess) {
		cout << "Minimization test successful" << endl;
	}
	else {
		cout << "Minimization test failed" << endl;
	}
	cout << "Minimization test complete" << endl;
}
