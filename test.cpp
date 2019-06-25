#include <iostream>
#include <string>
#include "simpl.h"

#define NUMVAR 6
#define NUMCONSTR 7
#define NUMCOL 7
#define NUMROW 8

using namespace std;

int main () {
	simpl *test = new simpl(NUMVAR,NUMCONSTR);
	bool isErr = false;
	float** tableauCheck = new float*[NUMROW];
	int i,j;
	float testNum = -8.2;
	for (i = 0; i < NUMROW; i++) {
		tableauCheck[i] = new float[NUMCOL];
	}
	//initialize values of tableau
	for (i = 1; i <= NUMROW; i++) {
		for (j = 1; j <= NUMCOL; j++) {
			testNum = (i+j+1.0)/(i*j+1.0);
			if (!(test->changeValue(testNum,i,j))) {
				cout << "Error entering " << testNum << " into row " << i << " and " << j << endl;
				isErr = true;
			}
		}
	}
	//looking at the values in the tableau
	test->returnTableau(tableauCheck);
	for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}
	//pivot tableau
	if (!(test->pivot(1,1))) {
		cout << "Error: pivot returned false when it should return true" << endl;
		isErr = true;
	}
	//looking at the values in the tableau after pivoting
	test->returnTableau(tableauCheck);
	for (i = 0; i < NUMROW; i++) {
		for (j = 0; j < NUMCOL; j++) {
			cout << "Value in row " << (i+1) << " and column " << (j+1) << " is " << tableauCheck[i][j] << endl;
		}
	}
	if (!isErr) {
		cout << "All tests successfully passed" << endl;
	}
	//free all dynamic memory used
	delete test;
	test = NULL;
	for (i = 0; i < NUMROW; i++) {
		delete tableauCheck[i];
		tableauCheck[i] = NULL;
	}
	delete tableauCheck;
	tableauCheck = NULL;
	cout << "All good\n";
	return 0;
}
