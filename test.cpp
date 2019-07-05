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

int main () {
	pivotTest();
	simplTest();
	return 0;
}

void pivotTest() {
	simpl* test = new simpl(NUMVAR, NUMCONSTR);
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
	simpl* test = new simpl(NUMVAR, NUMCONSTR);
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
	test->simplex(testxVal, &testNum);
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