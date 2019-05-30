#include <iostream>
#include <string>
#include "simpl.h"

using namespace std;

int main () {
	simpl *test = new simpl(2,2);
	bool isErr = false;
	float** tableauCheck = new float*[3];
	int i,j;
	float testNum = -8.2;
	for (i = 0; i < 3; i++) {
		tableauCheck[i] = new float[3];
	}
	test->changeValue(testNum,1,2);
	if (!(test->changeValue(10.2,1,1))) {
		cout << "Error: changeValue returned false when it should return true" << endl;
		isErr = true;
	}
	if (test->changeValue(10.2,10,1)) {
		cout << "Error: changeValue returned true when it should return false" << endl;
		isErr = true;
	}
	if (test->changeValue(10.2,0,1)) {
		cout << "Error: changeValue returned true when it should return false" << endl;
		isErr = true;
	}
	test->returnTableau(tableauCheck);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	delete test;
	test = NULL;
	for (i = 0; i < 3; i++) {
		delete tableauCheck[i];
		tableauCheck[i] = NULL;
	}
	delete tableauCheck;
	tableauCheck = NULL;
	cout << "All good\n";
	return 0;
}
